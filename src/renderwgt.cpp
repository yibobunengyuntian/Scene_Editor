#include "renderwgt.h"

RenderWgt::RenderWgt(QWidget *parent) :
    QOpenGLWidget(parent)
{
    setAcceptDrops(true);  /* 允许拖放 */
    setFocusPolicy(Qt::ClickFocus);
    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    initialize();
}

void RenderWgt::initializeGL()
{
    // 创建着色器程序对象
    initializeOpenGLFunctions();

    initializeShader();

    g_PropertyManager->camera()->init();
    m_pCameraTool->setData(g_PropertyManager->camera()->getMoveSpeed(), g_PropertyManager->camera()->getCameraPos());

    createDepthTexture();

    //    开启剔除操作效果
    //    glEnable(GL_CULL_FACE);
    //禁用背面的的光照和阴影效果,避免不必要的计算
    glCullFace(GL_BACK);

    glEnable(GL_MULTISAMPLE);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);  // 可以根据需要设置不同的提示参数

    glEnable(GL_DEPTH_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    createFloor();
}

void RenderWgt::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);                //定义视口区域
    m_projection.setToIdentity();
    m_projection.perspective(45.0f,width()/float(height()),CAM_NEAR,CAM_FAR);
    g_PropertyManager->camera()->setProjection(m_projection);
    m_pViewCube->move(w - m_pViewCube->size().width(), 0);
}

void RenderWgt::paintGL()
{
    glClearColor(m_backgroundColor.x(), m_backgroundColor.y(), m_backgroundColor.z(), m_backgroundColor.w());   // 背景颜色
    // 开启模板测试
    glEnable(GL_STENCIL_TEST);
    // 指定如何更新模板缓冲区的值
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    // 不允许向模板缓冲区写入值
    glStencilMask(0x00);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    loadWorld();
    loadModel();

    if(g_PropertyManager->manipulator())
    {
        g_PropertyManager->manipulator()->setupGlFunc(QOpenGLContext::currentContext()->versionFunctions<GLFUNC>());
    }
    if(m_isCopyModel && g_PropertyManager->selectNode() != nullptr)
    {
        m_isCopyModel = false;
    }

    emit g_PropertyManager->renderLogic();

    if(m_isUseCameraDir)
    {
        m_tmpLightDir = g_PropertyManager->camera()->getDir().normalized();
    }
    else
    {
        m_tmpLightDir = m_lightDir.normalized();

    }

    renderDepthMap();
    renderNode();
}

void RenderWgt::wheelEvent(QWheelEvent *pEvent)
{
    int i = pEvent->angleDelta().y()/120;
    if(i < 0)
    {
        for(int j = 0; j < -i; ++j)
        {
            g_PropertyManager->camera()->moveCamera(CAMERA_MOVE_DIR::FRONT);
        }
    }
    else
    {
        for(int j = 0; j < i; ++j)
        {
            g_PropertyManager->camera()->moveCamera(CAMERA_MOVE_DIR::BACK);
        }
    }
    m_pCameraTool->setData(g_PropertyManager->camera()->getMoveSpeed(), g_PropertyManager->camera()->getCameraPos());
    if(m_shadowUpDate == 2)
    {
        m_isUpDateShadow = true;
    }
}

void RenderWgt::mousePressEvent(QMouseEvent *pEvent)
{
    switch (pEvent->button())
    {
    case Qt::LeftButton:
    {
        if(m_isMouseRightDown == false)
        {
            if(m_isCopyModel && m_pTmpNode)
            {
                m_pTmpNode->setHideMesh(true);
//                g_PropertyManager->addNode(m_pTmpNode);
                g_PropertyManager->undoStack()->push(new UndoCommandAddNode(m_pTmpNode, "add node::" + m_pTmpNode->name()));
                m_pTmpNode = g_PropertyManager->copyNode(m_pCopyNode);
                m_pTmpNode->setHideMesh(false);
                m_isLoadModel = true;
                return;
            }
            m_isMouseLeftDown = true;
            setMouseTracking(true);                 //开启鼠标追踪
            if(g_PropertyManager->manipulator() && g_PropertyManager->selectNode())
            {
                m_isSelectManipulator = g_PropertyManager->manipulator()->mouseDetection(this->mapFromGlobal(QCursor::pos()), this->width(), this->height());
                if(m_isSelectManipulator)
                {
                    g_PropertyManager->manipulator()->setMouesPressPos(this->mapFromGlobal(QCursor::pos()));
                    g_PropertyManager->manipulator()->setIsMouesPress(true);
                }
                //qDebug() << m_isSelectManipulator;
            }
            else
            {
                m_isMouseMoveCamera = true;
            }
        }
    }
        break;
    case Qt::RightButton:
    {
        m_oldDir = g_PropertyManager->camera()->getDir();
        m_mouseRightDownPos = QCursor::pos();
        setCursor(Qt::BlankCursor);             //隐藏鼠标光标
        //QCursor::setPos(geometry().center());   //将鼠标移动窗口中央
        setMouseTracking(true);                 //开启鼠标追踪

        m_isMouseLeftDown = false;
        m_isMouseRightDown = true;
    }
        break;
    default:
        break;
    }
}

void RenderWgt::mouseMoveEvent(QMouseEvent *pEvent)
{
    Q_UNUSED(pEvent)
    if(m_isMouseRightDown)
    {
        float xoffset = QCursor::pos().x() - m_mouseRightDownPos.x();
        float yoffset = m_mouseRightDownPos.y() - QCursor::pos().y();
        g_PropertyManager->camera()->setDir(xoffset, yoffset);
        QCursor::setPos(m_mouseRightDownPos);
        m_pViewCube->setModelMat(g_PropertyManager->camera()->getDir(), g_PropertyManager->camera()->getUp());
        return;
    }
    if(m_isCopyModel && m_pTmpNode)
    {
        QVector4D worldPostion=worldPosFromViewPort(pEvent->pos().x(),
            pEvent->pos().y());
        if(worldPostion.w() > 1.2)
        {
            m_pTmpNode->setHide(true);
            return;
        }
        m_pTmpNode->setTranslate(worldPostion.toVector3D());
        m_pTmpNode->setHide(false);
        return;
    }
    if(g_PropertyManager->manipulator() != nullptr && g_PropertyManager->selectNode() != nullptr)
    {
        g_PropertyManager->manipulator()->mouseDetection(this->mapFromGlobal(QCursor::pos()), this->width(), this->height());
        if(m_isSelectManipulator)
        {
            g_PropertyManager->manipulator()->transformByMouesmove(this->mapFromGlobal(QCursor::pos()), this->width(), this->height());
            g_PropertyManager->nodeEditWgt()->update();
        }
    }
    if(m_isMouseMoveCamera)
    {

    }
}

void RenderWgt::mouseReleaseEvent(QMouseEvent *pEvent)
{
    switch (pEvent->button())
    {
    case Qt::LeftButton:
    {
        m_isMouseLeftDown = false;
        m_isSelectManipulator = false;
        m_isMouseMoveCamera = false;

        if(g_PropertyManager->manipulator())
        {
            g_PropertyManager->manipulator()->setIsMouesPress(false);
        }
    }
    break;
    case Qt::RightButton:
    {
        QCursor::setPos(m_mouseRightDownPos);
        setCursor(cursor);   //恢复鼠标光标
        m_isMouseRightDown = false;
    }
    break;
    default:
        break;
    }
}

void RenderWgt::mouseDoubleClickEvent(QMouseEvent *pEvent)
{
    if(pEvent->button() == Qt::LeftButton)
    {
        if(m_isCopyModel && m_pTmpNode)
        {
            return;
        }
        //m_isMouseLeftDown = false;
        QVector4D worldPostion=worldPosFromViewPort(pEvent->pos().x(), pEvent->pos().y());

        QVector4D pos = m_projection * g_PropertyManager->camera()->getView() * worldPostion;
        pos/=pos.w();
        foreach(auto node, g_PropertyManager->nodes())
        {
            node->selectNode(worldPostion);
//            if(selectNode)
//            {
//                g_PropertyManager->selectNode(selectNode);
//                return;
//            }
        }
        if(g_PropertyManager->selectTmpNodes().size()>0)
        {
            g_PropertyManager->selectNode(g_PropertyManager->selectTmpNodes().first());
            g_PropertyManager->removeSelectTmpNodes();
            return;
        }
        g_PropertyManager->selectNode(nullptr);
    }
    if(pEvent->button() == Qt::RightButton)
    {
        if(m_isCopyModel && m_pTmpNode)
        {
            delete []m_pTmpNode;
            m_pTmpNode = nullptr;
            m_isCopyModel = false;
        }
        m_oldDir = g_PropertyManager->camera()->getDir();
        m_mouseRightDownPos = QCursor::pos();
        setCursor(Qt::BlankCursor);             //隐藏鼠标光标
        //QCursor::setPos(geometry().center());   //将鼠标移动窗口中央
        setMouseTracking(true);                 //开启鼠标追踪

        m_isMouseLeftDown = false;
        m_isMouseRightDown = true;
    }
}

void RenderWgt::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Control:
    {
        m_isKeyCtrlDown = true;
        m_keys[Qt::Key_Control] = true;
    }
        break;
    case Qt::Key_C:
    {
        if(m_isKeyCtrlDown && g_PropertyManager->selectNode() != nullptr)
        {
            m_isCopyModel = true;
            m_pCopyNode = g_PropertyManager->selectNode();
            m_pTmpNode = g_PropertyManager->copyNode(m_pCopyNode);
            m_pTmpNode->setHideMesh(false);
            m_isLoadModel = true;
            g_PropertyManager->selectNode(nullptr);
        }
    }
        break;
    case Qt::Key_Z:
    {
        if(m_isKeyCtrlDown)
        {
            g_PropertyManager->undoStack()->undo();
        }
    }
        break;
    case Qt::Key_Y:
    {
        if(m_isKeyCtrlDown)
        {
            g_PropertyManager->undoStack()->redo();
        }
    }
        break;
    case Qt::Key_Delete:
    {
        if(g_PropertyManager->selectNode() != nullptr)
        {
            Node *node = g_PropertyManager->selectNode();
            g_PropertyManager->selectNode(nullptr);
//            g_PropertyManager->delNode(node);
            g_PropertyManager->undoStack()->push(new UndoCommandDelNode(node, "del node::" + node->name()));
        }
    }
        break;
    case Qt::Key_W:
    {
        m_keys[Qt::Key_W] = true;
        m_keys[Qt::Key_S] = false;
    }
        break;
    case Qt::Key_S:
    {
        m_keys[Qt::Key_S] = true;
        m_keys[Qt::Key_W] = false;
    }
        break;
    case Qt::Key_A:
    {
        m_keys[Qt::Key_A] = true;
        m_keys[Qt::Key_D] = false;
    }
        break;
    case Qt::Key_D:
    {
        m_keys[Qt::Key_D] = true;
        m_keys[Qt::Key_A] = false;
    }
        break;
    case Qt::Key_E:
    {
        m_keys[Qt::Key_E] = true;
        m_keys[Qt::Key_Q] = false;
    }
    break;
    case Qt::Key_Q:
    {
        m_keys[Qt::Key_Q] = true;
        m_keys[Qt::Key_E] = false;
    }
    break;
    case Qt::Key_Up:
    {
        m_keys[Qt::Key_Up] = true;
        m_keys[Qt::Key_Down] = false;
    }
    break;
    case Qt::Key_Down:
    {
        m_keys[Qt::Key_Down] = true;
        m_keys[Qt::Key_Up] = false;
    }
    break;
    case Qt::Key_Left:
    {
        m_keys[Qt::Key_Left] = true;
        m_keys[Qt::Key_Right] = false;
    }
    break;
    case Qt::Key_Right:
    {
        m_keys[Qt::Key_Right] = true;
        m_keys[Qt::Key_Left] = false;
    }
    break;
    default:
        break;
    }
    bool isShadowUpDate = false;
    for (QMap<Qt::Key, bool>::const_iterator it = m_keys.constBegin(); it != m_keys.constEnd(); it++)
    {
        if(it.value())
        {
            switch (it.key())
            {
            case Qt::Key_W:
            {
                g_PropertyManager->camera()->moveCamera(CAMERA_MOVE_DIR::FRONT);
                isShadowUpDate = true;
            }
                break;
            case Qt::Key_S:
            {
                g_PropertyManager->camera()->moveCamera(CAMERA_MOVE_DIR::BACK);
                isShadowUpDate = true;
            }
                break;
            case Qt::Key_A:
            {
                g_PropertyManager->camera()->moveCamera(CAMERA_MOVE_DIR::LEFT);
                isShadowUpDate = true;
            }
                break;
            case Qt::Key_D:
            {
                g_PropertyManager->camera()->moveCamera(CAMERA_MOVE_DIR::RIGHT);
                isShadowUpDate = true;
            }
                break;
            case Qt::Key_E:
            {
                g_PropertyManager->camera()->moveCamera(CAMERA_MOVE_DIR::UP);
                isShadowUpDate = true;
            }
            break;
            case Qt::Key_Q:
            {
                g_PropertyManager->camera()->moveCamera(CAMERA_MOVE_DIR::DOWN);
                isShadowUpDate = true;
            }
            break;
            case Qt::Key_Up:
            {
                g_PropertyManager->camera()->setDir(0, 5);
                m_pViewCube->setModelMat(g_PropertyManager->camera()->getDir(), g_PropertyManager->camera()->getUp());
            }
            break;
            case Qt::Key_Down:
            {
                g_PropertyManager->camera()->setDir(0, -5);
                m_pViewCube->setModelMat(g_PropertyManager->camera()->getDir(), g_PropertyManager->camera()->getUp());
            }
            break;
            case Qt::Key_Left:
            {
                g_PropertyManager->camera()->setDir(-5, 0);
                m_pViewCube->setModelMat(g_PropertyManager->camera()->getDir(), g_PropertyManager->camera()->getUp());
            }
            break;
            case Qt::Key_Right:
            {
                g_PropertyManager->camera()->setDir(5, 0);
                m_pViewCube->setModelMat(g_PropertyManager->camera()->getDir(), g_PropertyManager->camera()->getUp());
            }
            break;
            default:
                break;
            }
        }
        m_pCameraTool->setData(g_PropertyManager->camera()->getMoveSpeed(), g_PropertyManager->camera()->getCameraPos());
    }
    if(isShadowUpDate == true && m_shadowUpDate == 2)
    {
        m_isUpDateShadow = true;
    }
}

void RenderWgt::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Control:
    {
        m_isKeyCtrlDown = false;
        m_keys[Qt::Key_Control] = false;
    }
        break;
    case Qt::Key_W:
    {
        m_keys[Qt::Key_W] = false;
    }
        break;
    case Qt::Key_S:
    {
        m_keys[Qt::Key_S] = false;
    }
        break;
    case Qt::Key_A:
    {
        m_keys[Qt::Key_A] = false;
    }
        break;
    case Qt::Key_D:
    {
        m_keys[Qt::Key_D] = false;
    }
        break;
    case Qt::Key_E:
    {
        m_keys[Qt::Key_E] = false;
    }
    break;
    case Qt::Key_Q:
    {
        m_keys[Qt::Key_Q] = false;
    }
    break;
    case Qt::Key_Up:
    {
        m_keys[Qt::Key_Up] = false;
    }
    break;
    case Qt::Key_Down:
    {
        m_keys[Qt::Key_Down] = false;
    }
    break;
    case Qt::Key_Left:
    {
        m_keys[Qt::Key_Left] = false;
    }
    break;
    case Qt::Key_Right:
    {
        m_keys[Qt::Key_Right] = false;
    }
    break;
    default:
        break;
    }
}

void RenderWgt::dropEvent(QDropEvent *event)
{
    Q_UNUSED(event)
    if(m_pTmpNode)
    {
//        g_PropertyManager->addNode(m_pTmpNode);
        g_PropertyManager->undoStack()->push(new UndoCommandAddNode(m_pTmpNode, "add node::" + m_pTmpNode->name()));
        g_PropertyManager->selectNode(m_pTmpNode);
        m_pTmpNode = nullptr;
    }
}

void RenderWgt::dragEnterEvent(QDragEnterEvent *event)
{
    if (!event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist"))
    {
        event->ignore();
        return;
    }
    QListWidget *pListwidget = qobject_cast<QListWidget *>(event->source());
    PropertyFile propertyFile = g_PropertyManager->propertysWgt()->propertyFile(pListwidget->currentItem());
    if(propertyFile.path == "" || propertyFile.type != 1)
    {
        event->ignore();
        return;
    }
    g_PropertyManager->selectNode(nullptr);
    event->acceptProposedAction();
    m_modelPath = propertyFile.path;
    if(!m_modelPath.isEmpty())
    {
        m_pTmpNode = g_PropertyManager->loadModel(m_modelPath);
        m_pTmpNode->setHideMesh(false);
        m_loadModelMutex.lock();
        m_isLoadModel = true;
        m_loadModelMutex.unlock();
    }
}

void RenderWgt::dragMoveEvent(QDragMoveEvent *event)
{
    if(!m_pTmpNode)
    {
        event->ignore();
        return;
    }
    QVector4D worldPostion=worldPosFromViewPort(event->pos().x(),
        event->pos().y());
    // qDebug() << worldPostion;
    if(worldPostion.w() > 1.2)
    {
        m_pTmpNode->setHide(true);
        event->ignore();
        return;
    }
    event->acceptProposedAction();
    m_pTmpNode->setTranslate(worldPostion.toVector3D());
    m_pTmpNode->setHide(false);
}

void RenderWgt::dragLeaveEvent(QDragLeaveEvent *event)
{
    Q_UNUSED(event)
    if(m_pTmpNode)
    {
        delete []m_pTmpNode;
        m_pTmpNode = nullptr;
    }
}

void RenderWgt::initialize()
{
    setMinimumSize(100, 100);
    QPixmap pixmap;
    pixmap.load(":/res/icon/mou.png");
    pixmap.setDevicePixelRatio(1.8);
    cursor = QCursor(pixmap, 2, 2);
    setCursor(cursor);

    m_pViewCube = new ViewCube(parentWidget());
//    m_pViewCube->show();
    m_pCameraTool = new CameraTool(this);

    m_pTip = new TipWgt(this);
    m_pTip->raise();
    m_pTip->move(10, 30);
    m_pTip->hide();
    //创建显示进度的任务栏按钮
    connect(m_pCameraTool,&CameraTool::dataChange,[=](float speed, QVector3D pos){
        g_PropertyManager->camera()->setMoveSpeed(speed);
        g_PropertyManager->camera()->setCameraPos(pos);
    });
    connect(m_pCameraTool, &CameraTool::cameraRes, [=](){
        g_PropertyManager->camera()->res();
        m_pCameraTool->setData(g_PropertyManager->camera()->getMoveSpeed(), g_PropertyManager->camera()->getCameraPos());
        m_pViewCube->setModelMat(g_PropertyManager->camera()->getDir(), g_PropertyManager->camera()->getUp());
    });

//    timer.setInterval(1);
    connect(&timer,&QTimer::timeout,this,static_cast<void (RenderWgt::*)()>(&RenderWgt::update));
    connect(&timer,&QTimer::timeout,this,[=](){
        static QTime time(QTime::currentTime());//
        double key = time.elapsed()/1000.0;
        //this->replot();
        static double lastFpsKey = 0;
        static int  frameCount;
        ++frameCount;
        if(key - lastFpsKey>1){
            m_pCameraTool->setFPS(QString("%1 FPS").arg(frameCount/(key-lastFpsKey), 0, 'f', 0));
            lastFpsKey = key;
            frameCount = 0;
        }
        g_PropertyManager->setFps(frameCount/(key-lastFpsKey));
    });
    timer.start();
    m_pViewCube->setModelMat(g_PropertyManager->camera()->getDir(), g_PropertyManager->camera()->getUp());

//    QSurfaceFormat newGLFormat = this->format();  //开启抗锯齿(开启后二维转三维坐标问题未解决)
//    newGLFormat.setSamples(4);
    //    this->setFormat(newGLFormat);
}

void RenderWgt::initializeShader()
{
    bool success;
    m_ShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,":/res/shader/shapes.vert");
    m_ShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,":/res/shader/shapes.frag");
    success=m_ShaderProgram.link();
    if(!success) qDebug()<<"ERR:"<<m_ShaderProgram.log();

    m_DepthMapShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,":/res/shader/depthMap.vert");
    m_DepthMapShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,":/res/shader/depthMap.frag");
    success=m_DepthMapShaderProgram.link();
    if(!success) qDebug()<<"ERR:"<<m_DepthMapShaderProgram.log();
}

void RenderWgt::createFloor()
{
    if(g_PropertyManager->floor())
    {
        return;
    }
    QFile file(qApp->applicationDirPath() + "/floor.obj");
    QFile::copy(":/res/model/floor/floor.obj", qApp->applicationDirPath() + "/floor.obj");
    QFile::copy(":/res/model/floor/floor.png", qApp->applicationDirPath() + "/floor.png");
    QFile::copy(":/res/model/floor/floor.mtl", qApp->applicationDirPath() + "/floor.mtl");
    Node* floor = g_PropertyManager->loadModel(file.fileName(), false);
    floor->setUpTextureId();
    floor->setupGlFunc(QOpenGLContext::currentContext()->versionFunctions<GLFUNC>());
    floor->setHideMesh(true);
    floor->setScale(QVector3D(1000, 0.01f, 1000));
    g_PropertyManager->setFloor(floor);
    g_PropertyManager->propertysWgt()->removeAll();
}

void RenderWgt::createDepthTexture()
{
    // configure depth map FBO
    // -----------------------
    glGenFramebuffers(1, &m_depthMapFBO);
    // create depth texture
    glGenTextures(1, &m_depthMap);
    glBindTexture(GL_TEXTURE_2D, m_depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_SHADOW_WIDTH, m_SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        qDebug() << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject() );
}

void RenderWgt::loadWorld()
{
    if(!g_PropertyManager->m_isLoadWorld)
    {
        return;
    }
    foreach(auto node, g_PropertyManager->m_tmpNodes)
    {
        node->setupGlFunc(QOpenGLContext::currentContext()->versionFunctions<GLFUNC>());
        g_PropertyManager->addNode(node);
        g_PropertyManager->m_tmpNodes.removeOne(node);
        int num = g_PropertyManager->nodes().size() * 100/(g_PropertyManager->m_tmpNodes.size() + g_PropertyManager->nodes().size());
        g_PropertyManager->loadWgt()->onSetTip(QString(QStringLiteral("即将加载完成 %1 %")).arg(num));
        emit g_PropertyManager->setTaskbarProgressValue(60 + num*0.4);
        if(g_PropertyManager->m_tmpNodes.size() == 0)
        {
            g_PropertyManager->loadWgt()->onSetTip(QStringLiteral("加载完成!"));
            g_PropertyManager->m_isLoadWorld = false;
            g_PropertyManager->m_tmpNodes.clear();
            g_PropertyManager->loadWgt()->hide();
            g_PropertyManager->setIsNeedSave(false);
            emit g_PropertyManager->setEnabledSignal(true);
            emit g_PropertyManager->setTaskbarProgressValue(0);
            QApplication::alert(this);
            if(m_shadowUpDate == 2)
            {
                m_isUpDateShadow = true;
            }
        }
        break;
    }
}

void RenderWgt::loadModel()
{
    if(!m_isLoadModel)
    {
        return;
    }
    m_isLoadModel = false;
    if(m_pLoadModel)
    {
        m_pLoadModel->setupGlFunc(QOpenGLContext::currentContext()->versionFunctions<GLFUNC>());
//        g_PropertyManager->addNode(m_pLoadModel);
        g_PropertyManager->undoStack()->push(new UndoCommandAddNode(m_pLoadModel, "add node::" + m_pLoadModel->name()));
        g_PropertyManager->selectNode(m_pLoadModel);
        m_pLoadModel = nullptr;
    }
    if(m_pTmpNode)
    {
        m_pTmpNode->setUpTextureId();
        m_pTmpNode->setupGlFunc(QOpenGLContext::currentContext()->versionFunctions<GLFUNC>());
    }
    g_PropertyManager->setEnabledSignal(true);
    m_pTip->hide();
    if(m_shadowUpDate == 2)
    {
        m_isUpDateShadow = true;
    }
}

void RenderWgt::renderDepthMap()
{
    // m_isDepthMap = true;
    if(m_isDepthMap)
    {
        if(m_isUpDateShadow)
        {
            if(m_shadowUpDate != 1)
            {
                m_isUpDateShadow = false;
            }

            m_DepthMapShaderProgram.bind();
            // 1. render depth of scene to texture (from light's perspective)
            // --------------------------------------------------------------
            QMatrix4x4 lightProjection, lightView;
            QMatrix4x4 lightSpaceMatrix;
            float near_plane = 10.0f, far_plane = 500.0f;
            lightProjection.ortho(-1000.0f, 1000.0f, -1000.0f, 1000.0f, near_plane, far_plane);
            lightView.lookAt(-m_tmpLightDir*100 + g_PropertyManager->camera()->getCameraPos(),
                             g_PropertyManager->camera()->getCameraPos(), QVector3D(0.0, 1.0, 0.0));
            lightSpaceMatrix = lightProjection * lightView;

            glViewport(0, 0, m_SHADOW_WIDTH, m_SHADOW_HEIGHT);
            glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
            glClear(GL_DEPTH_BUFFER_BIT);
            m_DepthMapShaderProgram.setUniformValue("lightSpaceMatrix", lightSpaceMatrix);

            // glEnable(GL_CULL_FACE);
            // glCullFace(GL_FRONT);
            foreach(auto node, g_PropertyManager->nodes())
            {
                node->Draw(m_DepthMapShaderProgram, true);
            }
            // g_PropertyManager->floor()->Draw(m_DepthMapShaderProgram, true);
            // glCullFace(GL_BACK); // 不要忘记设回原先的culling face
            // glDisable(GL_CULL_FACE);
            glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());
            m_DepthMapShaderProgram.release();
            m_ShaderProgram.bind();
            m_ShaderProgram.setUniformValue("lightSpaceMatrix", lightSpaceMatrix);
            glViewport(0, 0, width(), height());
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, m_depthMap);
            m_ShaderProgram.setUniformValue("depthMap",1);
            m_ShaderProgram.setUniformValue("isDepthMap",true);
        }
    }
    else
    {
        // glClear(GL_DEPTH_BUFFER_BIT);
        m_ShaderProgram.bind();
        m_ShaderProgram.setUniformValue("isDepthMap",false);
    }
}

void RenderWgt::renderNode()
{
    m_ShaderProgram.bind();

    m_ShaderProgram.setUniformValue("projection", m_projection);
    m_ShaderProgram.setUniformValue("view", g_PropertyManager->camera()->getView());

    m_ShaderProgram.setUniformValue("viewPos",g_PropertyManager->camera()->getCameraPos());
    m_ShaderProgram.setUniformValue("light.direction", m_tmpLightDir);

    g_PropertyManager->floor()->Draw(m_ShaderProgram);
    foreach(auto node, g_PropertyManager->nodes())
    {
        node->Draw(m_ShaderProgram);
    }
    if(m_pTmpNode)
    {
        glDisable(GL_DEPTH_TEST);
        m_pTmpNode->Draw(m_ShaderProgram);
        glEnable(GL_DEPTH_TEST);
    }
    if(g_PropertyManager->manipulator())
    {
        g_PropertyManager->manipulator()->Draw(m_ShaderProgram);
    }
    m_ShaderProgram.release();
}

QVector4D RenderWgt::worldPosFromViewPort(int posX, int posY)
{
    makeCurrent();
    float winZ;
    glReadPixels(
        posX,
        this->height()-posY
        ,1,1
        ,GL_DEPTH_COMPONENT,GL_FLOAT
        ,&winZ);
    doneCurrent();
    float x=(2.0f*posX)/this->width()-1.0f; //转化为标准设备坐标(-1,1)
    float y=1.0f-(2.0f*posY)/this->height();//转化为标准设备坐标(-1,1)
    float z=winZ*2.0f-1.0f;//转化为标准设备坐标(-1,1)

    float w = (2.0f * CAM_NEAR * CAM_FAR) / (CAM_FAR + CAM_NEAR - z * (CAM_FAR - CAM_NEAR));//计算齐次坐标
    //float w= _near*_far/(_near*winZ-_far*winZ+_far);
    QVector4D wolrdPostion(x,y,z,1);
    wolrdPostion=w*wolrdPostion;//裁剪空间的坐标
    return g_PropertyManager->camera()->getView().inverted()*m_projection.inverted()*wolrdPostion; //获得世界空间的坐标
}

void RenderWgt::setBackgroundColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    m_backgroundColor = QVector4D(r, g, b, a);
}

void RenderWgt::addModelByFile()
{
    m_modelPath = QFileDialog::getOpenFileName(this, QStringLiteral("导入模型"),
                                               g_PropertyManager->config()->Get("path", "model").toString(),
                                               tr("Model Files (%1)").arg(MODEL_FILE_TYPE));
    if(m_modelPath.isEmpty())
    {
        return;
    }
    g_PropertyManager->setEnabledSignal(false);
    m_pTip->setText(QStringLiteral("模型加载中..."));
    m_pTip->show();
    QFutureWatcher<void> *pWatcher = new QFutureWatcher<void>;
    QFuture<void> future = QtConcurrent::run([=]()
    {
        Node *node = g_PropertyManager->loadModel(m_modelPath);
        while(m_pLoadModel != nullptr);
        m_pLoadModel = node;
    });

    connect(pWatcher, &QFutureWatcher<void>::finished,this,[=](){
        m_pLoadModel->setUpTextureId();
        m_isLoadModel = true;
    });
    pWatcher->setFuture(future);
    QString dir = m_modelPath;
    dir.chop(dir.split("/").last().count());
    g_PropertyManager->config()->Set("path", "model", dir);
}
