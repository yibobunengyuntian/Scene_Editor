#include "mainwgt.h"
#include "ui_mainwgt.h"

MainWgt::MainWgt(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    // �����ޱ߿�
    setWindowFlags(Qt::FramelessWindowHint);//�ޱ߿�

    initWidget();
}

MainWgt::~MainWgt()
{

}

void MainWgt::setTaskbarProgressValue(int value)
{
    if(value == 0)
    {
        m_pWindowsTaskbarButton->setWindow(windowHandle());
        m_pWindowsTaskbarProgress = m_pWindowsTaskbarButton->progress();
        m_pWindowsTaskbarProgress->setRange(0, 100);
    }

    m_pWindowsTaskbarProgress->setValue(value);
    m_pWindowsTaskbarProgress->show();
}
// �˳���ť�����¼�
void MainWgt::quitEvent()
{
    if(g_PropertyManager->isNeedSave())
    {
        if(QMessageBox::question(this, QStringLiteral("��ʾ"),
                                 QStringLiteral("��ǰ��Ŀδ����, "
                                    "�Ƿ񱣴�?")) == QMessageBox::Yes)
        {
            g_PropertyManager->saveWorld();
        }
    }
    qApp->quit();
}
// �����С�����ڰ�ť�¼�
void MainWgt::mid_maxEvent()
{
    bool is = !this->isMaximized();
    leftTopLabel->setHidden(is);
    topLabel->setHidden(is);
    rightTopLabel->setHidden(is);
    leftLabel->setHidden(is);
    rightLabel->setHidden(is);
    leftBottomLabel->setHidden(is);
    bottomLabel->setHidden(is);
    rightBottomLabel->setHidden(is);
    if(this->isMaximized())
    {
          showNormal();
    }
    else {
        showMaximized();
    }
}
// ��С�������¼�
void MainWgt::minEvent()
{

     showMinimized();
}
// ���ڰ�ť�����¼�
void MainWgt::aboutEvent()
{

}
// �����¼�
void MainWgt::configEvent()
{

}
// ������Ϣ�¼�
void MainWgt::personEvent()
{

}

// �϶����ڱ��⣬�ƶ�����
void MainWgt::moveWindowEvent(QPoint& pos)
{
    if(this->isMaximized()) return;
    QPoint tmp = this->pos() + pos;
    this->move(tmp);
}

void MainWgt::onModeChanged(bool is)
{
    g_PropertyManager->setIsDrawMesh(is);
}

void MainWgt::onLightWgtBtnClicked()
{
    m_pLightWgt->move(pos() + pToolBar->pos() + QPoint(m_pLightWgtBtn->pos().x() - m_pLightWgt->width(), pToolBar->height()));
    if(m_pLightWgt->isHidden())
    {
        m_pLightWgtBtn->setDown(true);
//        Qt::WindowFlags m_flags = windowFlags();
        //m_pWidgetSelectMenu->setWindowFlags(m_flags | Qt::WindowStaysOnTopHint);
        m_pLightWgt->show();
    }
    else
    {
        m_pLightWgt->hide();
    }
}

void MainWgt::onNew()
{
    NewProjectWgt *pNew = new NewProjectWgt();
    pNew->show();
    //...
}

void MainWgt::onOPen()
{
    if(g_PropertyManager->isNeedSave())
    {
        if(QMessageBox::question(this, QStringLiteral("��ʾ"),
                                 QStringLiteral("��ǰ��Ŀδ����, "
                                    "�Ƿ񱣴�?")) == QMessageBox::Yes)
        {
            g_PropertyManager->saveWorld();
        }
    }
    QString proDir = QFileDialog::getOpenFileName(this, QStringLiteral("���س���"),
                                                  g_PropertyManager->config()->Get("path", "world").toString(),
                                                  "world file (*.world)");
    if(proDir.isEmpty())
    {
        return;
    }
    QString dir = proDir;
    dir.chop(dir.split("/").last().count());
    g_PropertyManager->config()->Set("path", "world", dir);
    g_PropertyManager->openWorld(proDir);
}

void MainWgt::onSave()
{
    //...
    g_PropertyManager->saveWorld();
}
void MainWgt::initWidget()
{
    m_pRenderWgt = new RenderWgt(widget);
    QSplitter *ren_pro_splitter = new QSplitter();
//    ren_pro_splitter->setChildrenCollapsible(0);
    ren_pro_splitter->setOrientation(Qt::Vertical);
    ren_pro_splitter->setHandleWidth(2);
    ren_pro_splitter->addWidget(m_pRenderWgt);
    ren_pro_splitter->addWidget(g_PropertyManager->propertysWgt());
    ren_pro_splitter->setStretchFactor(0, 1);
    ren_pro_splitter->setCollapsible(0, false);
    QSplitter *H_splitter = new QSplitter();
    H_splitter->setHandleWidth(2);
    H_splitter->addWidget(ren_pro_splitter);
    Layout->addWidget(H_splitter);

    QSplitter *V_splitter = new QSplitter();
    V_splitter->setOrientation(Qt::Vertical);
    V_splitter->setHandleWidth(2);
    V_splitter->addWidget(g_PropertyManager->nodesTreeWgt());
    V_splitter->addWidget(g_PropertyManager->nodeEditWgt());
    V_splitter->setStretchFactor(0, 1);
    H_splitter->addWidget(V_splitter);
    H_splitter->setStretchFactor(0,1);
    H_splitter->setCollapsible(0, false);
    m_pTiteBar = new TiteBar();
    m_pTiteBar->setWindowTitle(QStringLiteral("Scene_Editor"));
    m_pTiteBar->setIcon(":/res/icon/tite.png");
    this->setWindowTitle(QStringLiteral("Scene_Editor"));
    tite->addWidget(m_pTiteBar);

    pToolBar = new QToolBar;
    pToolBar->setFixedHeight(30);
    pToolBar->setStyleSheet(" QToolBar {\
                                background-color: rgb(75, 75, 75);\
                                border:0px;\
                                spacing:5px;\
                            }\
                            QToolBar QToolButton{\
                                min-width: 20px;\
                                min-height: 20px;\
                                background-color: transparent;\
                            }\
                            QToolBar QToolButton:hover{\
                                background-color: rgba(255, 255, 255, 0.1);\
                            }\
                            QToolBar QToolButton:pressed{\
                                background-color: rgba(255, 255, 255, 0.15);\
                            }\
                            QComboBox:drop-down{\
                                border-style:none\
                            }\
" + m_styleSheet);
    QAction *pOpen = pToolBar->addAction(QIcon(":/res/icon/open.png"),"");
    pOpen->setToolTip(QStringLiteral("�򿪳���"));
//    QAction *pNew = pToolBar->addAction(QIcon(":/res/icon/add.png"),"");
//    pNew->setToolTip(QStringLiteral("�½�����"));
    //    pNew->setShortcut(QKeySequence("Ctrl+N"));
    QAction *pSave = pToolBar->addAction(QIcon(":/res/icon/save.png"),"");
    pSave->setToolTip(QStringLiteral("���泡��"));
    pSave->setShortcut(QKeySequence("Ctrl+S"));
    QAction *pAddModel = pToolBar->addAction(QIcon(":/res/icon/addmodel.png"),"");
    pAddModel->setToolTip(QStringLiteral("���ģ��"));

    pToolBar->addSeparator();
    QComboBox *manipulator_type = new QComboBox();
    manipulator_type->setStyleSheet("color: rgb(255, 255, 255);\
                                    background-color: rgb(118, 118, 118);\
                                    border:0px;\
                                    font: 12px;\
                                    selection-background-color:rgb(75, 75, 75);\
                                    outline: 0px solid gray;\
                                    min-width:35px;\
                                    min-height:20px;\
");
    manipulator_type->addItem(QStringLiteral("ƽ��"));
    manipulator_type->addItem(QStringLiteral("��ת"));
    manipulator_type->addItem(QStringLiteral("����"));
    manipulator_type->setToolTip(QStringLiteral("ģ�Ͳ���"));
    pToolBar->addWidget(manipulator_type);

    QComboBox *manipulatorReferenceSystem = new QComboBox();
    manipulatorReferenceSystem->setStyleSheet("color: rgb(255, 255, 255);\
                                    background-color: rgb(118, 118, 118);\
                                    border:0px;\
                                    font: 12px;\
                                    selection-background-color:rgb(75, 75, 75);\
                                    outline: 0px solid gray;\
                                    min-width:35px;\
                                    min-height:20px;\
");
    manipulatorReferenceSystem->addItem(QStringLiteral("����"));
    manipulatorReferenceSystem->addItem(QStringLiteral("����"));
    //manipulatorReferenceSystem->addItem(QStringLiteral("���ڵ�"));
    manipulatorReferenceSystem->setToolTip(QStringLiteral("ģ�Ͳ����ο�ϵ"));
    pToolBar->addWidget(manipulatorReferenceSystem);

    QAction *pUndo = pToolBar->addAction(QIcon(":/res/icon/undo.png"),"");
    pUndo->setToolTip(QStringLiteral("����"));
    QAction *pRedo = pToolBar->addAction(QIcon(":/res/icon/redo.png"),"");
    pRedo->setToolTip(QStringLiteral("�ָ�"));
    pUndo->setEnabled(false);
    pRedo->setEnabled(false);

    pToolBar->addSeparator();
    QAction *pIsMesh = pToolBar->addAction(QIcon(":/res/icon/mesh.png"),"");
    pIsMesh->setToolTip(QStringLiteral("������ʾ"));
    pIsMesh->setCheckable(true);
    pIsMesh->setChecked(false);
    pToolBar->widgetForAction(pIsMesh)->setStyleSheet(g_CHECKTOOLSTYLE);

    QList<MenuItem*> listMenuitem;

    MenuItem *pMenuitemAABB = new MenuItem(0,QStringLiteral("��Χ��"),g_PropertyManager->nodeSelectState().value(SHOWAABB).toBool());
    listMenuitem.append(pMenuitemAABB);
    MenuItem *pMenuitemMesh = new MenuItem(1,QStringLiteral("����"),g_PropertyManager->nodeSelectState().value(SHOWMESH).toBool());
    listMenuitem.append(pMenuitemMesh);
    MenuItem *pMenuitemOutline = new MenuItem(2,QStringLiteral("����"),g_PropertyManager->nodeSelectState().value(SHOWOUTLINE).toBool());
    listMenuitem.append(pMenuitemOutline);
    MultiComBox *pMenuItem = new MultiComBox;
    pMenuItem->setText(QStringLiteral("ѡ�б���"));
    pMenuItem->AddItems(listMenuitem);
    pMenuItem->setStyleSheet("color: rgb(255, 255, 255);\
                                    background-color: rgb(118, 118, 118);\
                                    border:0px;\
                                    font: 12px;\
                                    selection-background-color:rgb(75, 75, 75);\
                                    outline: 0px solid gray;\
                                    min-width:50px;\
                                    min-height:20px;\
");
    pToolBar->addWidget(pMenuItem);

    QAction *pIsShadow = pToolBar->addAction(QIcon(":/res/icon/shadow.png"),"");
    pIsShadow->setToolTip(QStringLiteral("��Ӱ(������Ӱ����ʧ�ϴ�����)"));
    pIsShadow->setCheckable(true);
    pIsShadow->setChecked(false);
    pToolBar->widgetForAction(pIsShadow)->setStyleSheet(g_CHECKTOOLSTYLE);
    QComboBox *pShadowUpDate = new QComboBox();
    pShadowUpDate->setStyleSheet("color: rgb(255, 255, 255);\
                                 background-color: rgb(118, 118, 118);\
                                 border:0px;\
                                 font: 12px;\
                                 selection-background-color:rgb(75, 75, 75);\
                                 outline: 0px solid gray;\
                                 min-width:55px;\
                                 min-height:20px;\
                                 ");

    QStandardItemModel *pModel = new QStandardItemModel();//�����ʾtootip

    QStandardItem *item = new QStandardItem(QStringLiteral("�ֶ�����"));
    item->setToolTip(QStringLiteral("������Ӱ������"));
    pModel->appendRow(item);
    item = new QStandardItem(QStringLiteral("ʵʱ����"));
    item->setToolTip(QStringLiteral("������ʧ��"));
    pModel->appendRow(item);
    item = new QStandardItem(QStringLiteral("�����ı�"));
    item->setToolTip(QStringLiteral("�����ı�ʱӰ��(�Ƽ�)"));
    pModel->appendRow(item);
    pShadowUpDate->setModel(pModel);
    pShadowUpDate->setToolTip(QStringLiteral("��Ӱ����"));
    pToolBar->addWidget(pShadowUpDate);
//    pToolBar->addSeparator();

    QWidget *spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pToolBar->addWidget(spacer);

    QtColorButton *pColorBtn = new QtColorButton;
    pColorBtn->setToolTip(QStringLiteral("����ɫ"));
    pColorBtn->setColor(QColor(0*255, 0.4f*255, 0.6f*255, 1.0f*255));
    pToolBar->addWidget(pColorBtn);
    m_pLightWgtBtn = new QToolButton();
    m_pLightWgtBtn->setText(QStringLiteral("����"));
    m_pLightWgtBtn->setStyleSheet("QToolButton{"
                                      "color: white;"
                                      "outline: 0px solid gray;"
                                      "min-width:50px;"
                                      "min-height:20px;"
                                      "border: none;"
                                      "font: 12px;"
                                      "}"
                                      "QToolButton:hover {"
                                      "    background-color: rgb(118, 118, 118);"
                                      "border: none;"
                                      "}"
                                      "QToolButton:checked {"
                                      "    background-color: rgb(118, 118, 118);"
                                      "}");
    m_pLightWgt = new LightWgt(m_pLightWgtBtn, this);
    m_pLightWgt->hide();

    pToolBar->addWidget(m_pLightWgtBtn);
    toolBar->addWidget(pToolBar);

    //���ù�������8��Label��Ϊ�߿򣬹��˵�����¼�
    leftTopLabel->installEventFilter(this);
    topLabel->installEventFilter(this);
    rightTopLabel->installEventFilter(this);
    leftLabel->installEventFilter(this);
    rightLabel->installEventFilter(this);
    leftBottomLabel->installEventFilter(this);
    bottomLabel->installEventFilter(this);
    rightBottomLabel->installEventFilter(this);

    //��������߿�ı��С�Ĳ���
    mWp = unKnown; //��������߿��ʼ��־Ϊ unKnow�� ����һ��ö���ͱ���
    m_bPressed = false;  // ������갴�µ�״̬���������ڱ߿�Label���϶�����Ĳ���Ϊfalse.
    mouseEvent = nullptr;  //��������¼��ĳ�ʼֵ,Ĭ��Ϊnull

    m_pWindowsTaskbarButton = new QWinTaskbarButton(this);

    connect(m_pTiteBar,SIGNAL(quitEvent()),this,SLOT(quitEvent()));
    connect(m_pTiteBar,SIGNAL(mid_maxEvent()),this,SLOT(mid_maxEvent()));
    connect(m_pTiteBar,SIGNAL(minEvent()),this,SLOT(minEvent()));
    connect(m_pTiteBar,SIGNAL(aboutEvent()),this,SLOT(aboutEvent()));
    connect(m_pTiteBar,SIGNAL(configEvent()),this,SLOT(configEvent()));
    connect(m_pTiteBar,SIGNAL(personEvent()),this,SLOT(personEvent()));
    connect(m_pTiteBar,SIGNAL(moveWindow(QPoint&)),this,SLOT(moveWindowEvent(QPoint&)));
    connect(pAddModel,SIGNAL(triggered(bool)),m_pRenderWgt,SLOT(addModelByFile()));
    connect(pIsMesh,SIGNAL(triggered(bool)),this,SLOT(onModeChanged(bool)));
//    connect(m_pWidgetSelectBtn, SIGNAL(clicked()), this, SLOT(onWidgetSelectBtnClicked()));
    connect(m_pLightWgtBtn, SIGNAL(clicked()), this, SLOT(onLightWgtBtnClicked()));
    connect(m_pLightWgt, SIGNAL(lightDirChanged(QVector3D)), m_pRenderWgt, SLOT(setLightDir(QVector3D)));
    connect(m_pLightWgt, SIGNAL(isUseCameraDir(bool)), m_pRenderWgt, SLOT(setUseCameraDir(bool)));
    connect(pSave, SIGNAL(triggered(bool)), this, SLOT(onSave()));
    //connect(pNew, SIGNAL(triggered(bool)), this, SLOT(onNew()));
    connect(pOpen, SIGNAL(triggered(bool)), this, SLOT(onOPen()));
    connect(pIsShadow,SIGNAL(triggered(bool)),m_pRenderWgt,SLOT(setHiddenShadow(bool)));
    connect(pShadowUpDate, SIGNAL(currentIndexChanged(int)), m_pRenderWgt, SLOT(setShadowUpDate(int)));
    connect(g_PropertyManager, SIGNAL(setTaskbarProgressValue(int)), this, SLOT(setTaskbarProgressValue(int)));
    connect(pUndo, SIGNAL(triggered()), g_PropertyManager->undoStack(), SLOT(undo()));
    connect(pRedo, SIGNAL(triggered()), g_PropertyManager->undoStack(), SLOT(redo()));
    connect(g_PropertyManager->undoStack(), SIGNAL(canUndoChanged(bool)), pUndo, SLOT(setEnabled(bool)));
    connect(g_PropertyManager->undoStack(), SIGNAL(canRedoChanged(bool)), pRedo, SLOT(setEnabled(bool)));
    connect(g_PropertyManager, &PropertyManager::setEnabledSignal, this,[=](bool is){
        pToolBar->setEnabled(is);
    });
    PropertyManager *pPropertyManager = g_PropertyManager;
    connect(manipulator_type, static_cast<void(QComboBox::*)(const int)>(&QComboBox::currentIndexChanged), this, [=](int index){
        pPropertyManager->setManipulatorType((MANIPULATOR_TYPE)index);
    });
    connect(pMenuItem,&MultiComBox::sigActivated, this,[=](){
        QList<int> nlist = pMenuItem->GetItemsId();
        QVariantMap map;
        map.insert(SHOWAABB, nlist.contains(0));
        map.insert(SHOWMESH, nlist.contains(1));
        map.insert(SHOWOUTLINE, nlist.contains(2));
        g_PropertyManager->setNodeSelectState(map);
    });
    connect(pColorBtn, &QtColorButton::colorChanged, this,[=](QColor color){
        m_pRenderWgt->setBackgroundColor(color.red()/255.0f, color.green()/255.0f, color.blue()/255.0f, color.alpha()/255.0f);
    });

    m_pTiteBar->setFocus();
    g_PropertyManager->loadWgt()->setParent(this);
    g_PropertyManager->loadWgt()->initialize();
    g_PropertyManager->loadWgt()->hide();
}

//void MainWgt::paintEvent(QPaintEvent *event)
//{
//    // ��ͼ����
//    // �ػ洰�ڵı�����ɫ
//    QPalette palette = this->palette();
//    QColor color(100,100,100);
//    palette.setBrush(QPalette::Window, QBrush(color));
//    this->setPalette(palette);
//    // �ػ洰�ڵı߿��С
//    QPainter painter(this);
//    QRect rect = this->rect();
//    QPen pen;
//    pen.setWidth(5);
//    pen.setColor(Qt::black);
//    painter.setPen(pen);
//    painter.drawRect(rect);

//    QWidget::paintEvent(event);
//}

bool MainWgt::eventFilter(QObject *target, QEvent *event)
{
    // ��������¼�����갴�£����øı䴰�ڴ�С�ı�־Ϊ��갴��״̬��
         if(event->type() == QEvent::MouseButtonPress)
         {
             //����������߿���
             mWp = testLabelBorder(target->objectName());
            if(mWp == LabelBorderStatus::unKnown)
            {
               m_bPressed = false;
            }
            else {
                 m_bPressed = true;
                 mouseEvent = nullptr;
                 mouseEvent = static_cast<QMouseEvent*>(event);
                 if(mouseEvent )
                 {
                     // ��ȡ��갴�µĶ�λ��
                     pLast = mouseEvent->globalPos();
                     event->ignore();
                 }
            }
         }
         // ���������ͷ��¼��������϶���־λ��ʼ״̬��
         else if(event->type() == QEvent::MouseButtonRelease)
         {
             m_bPressed = false;
             mWp = LabelBorderStatus::unKnown;
         }
         // ��������¼�������ƶ��¼��������Ƿ��϶��߿�ı䴰�ڴ�С
         else if(event->type() == QEvent::MouseMove)
         {
             //�������Ƿ��£������϶��ı߿�����
             if(m_bPressed && mWp != LabelBorderStatus::unKnown)
              {
                 mouseEvent = nullptr;
                 mouseEvent = static_cast<QMouseEvent*>(event);
                 if(mouseEvent)
                 {
                     //��ȡ����϶���λ�ã�
                     QPoint ptemp = mouseEvent->globalPos();
                     // ������갴�²��϶��ĵ�λ��
                     ptemp = ptemp - pLast ;
                     //���浱ǰ����϶���λ�ã�������һ�μ����϶�λ��
                     pLast = mouseEvent->globalPos();
                     event->ignore();
                     // ���촰�ں���
                     moveAndResizeWindow(ptemp);
                 }
             }
         }
         return QWidget::eventFilter(target,event);
}

MainWgt::LabelBorderStatus MainWgt::testLabelBorder(QString _objectName)
{
    if(_objectName == "leftTopLabel") return LabelBorderStatus::toLeftTop;
    if(_objectName == "topLabel") return LabelBorderStatus::toTop;
    if(_objectName == "rightTopLabel") return LabelBorderStatus::toRightTop;
    if(_objectName == "leftLabel") return LabelBorderStatus::toLeft;
    if(_objectName == "rightLabel") return LabelBorderStatus::toRight;
    if(_objectName == "leftBottomLabel") return LabelBorderStatus::toLeftBottom;
    if(_objectName == "bottomLabel") return LabelBorderStatus::toBottom;
    if(_objectName == "rightBottomLabel") return LabelBorderStatus::toRightBottom;
    return LabelBorderStatus::unKnown;
}

void MainWgt::moveAndResizeWindow(QPoint &pos)
{
    if(this->isMaximized()) return;
    int x=0,y=0,w=0,h=0;
    // �����϶�����һ���߿�ȷ�����컹����С���ڡ�
    switch (mWp) {
     // ��߿�����
     case LabelBorderStatus::toLeft:
         x = this->pos().x() + pos.x();
         y = this->pos().y();
         w = this->size().width() - pos.x();
         h = this->size().height();
        break;
      // �ұ߿�����
     case LabelBorderStatus::toRight:
         x = this->pos().x();
         y = this->pos().y();
         w = this->size().width() + pos.x();
         h = this->size().height();
         break;
         // �ϱ߿�����
     case LabelBorderStatus::toTop:
         x = this->pos().x();
         y = this->pos().y() + pos.y();
         w = this->size().width() ;
         h = this->size().height() - pos.y();
         break;
         // �±߿�����
     case LabelBorderStatus::toBottom:
         x = this->pos().x();
         y = this->pos().y();
         w = this->size().width() ;
         h = this->size().height() + pos.y();
         break;
         //���ϽǱ�����
     case LabelBorderStatus::toRightTop:
         x = this->pos().x();
         y = this->pos().y() + pos.y();
         w = this->size().width() + pos.x() ;
         h = this->size().height() - pos.y();
         break;
         //���ϽǱ�����
     case LabelBorderStatus::toLeftTop:
         x = this->pos().x() + pos.x();
         y = this->pos().y() + pos.y();
         w = this->size().width() - pos.x() ;
         h = this->size().height() - pos.y();
         break;
         // ���½Ǳ�����
     case LabelBorderStatus::toRightBottom:
         x = this->pos().x();
         y = this->pos().y();
         w = this->size().width() + pos.x() ;
         h = this->size().height() + pos.y();
         break;
         // ���½Ǳ�����
     case LabelBorderStatus::toLeftBottom:
         x = this->pos().x() + pos.x();
         y = this->pos().y();
         w = this->size().width() - pos.x() ;
         h = this->size().height() + pos.y();
         break;
     default:
         return;
     }
    // �ı䴰�ڵĴ�С��λ�á�
     if(w < this->minimumWidth())
     {
         x = this->pos().x();
         w = this->size().width();
     }
     if(h < this->minimumHeight())
     {
         y = this->pos().y();
         h = this->size().height();
     }
     this->setGeometry(x,y,w,h);
}
