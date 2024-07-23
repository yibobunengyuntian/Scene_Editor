#include "propertymanager.h"

PropertyManager* PropertyManager::m_propertyManager = nullptr;
uint PropertyManager::getTextureId(const QString &path, bool addtoProperty)
{
    foreach(auto tex, m_texturePropertys)
    {
        if(tex.path == path.toStdString())
        {
            return tex.id;
        }
    }
    QOpenGLTexture * texture=new QOpenGLTexture(QImage(path).mirrored());
    texture->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear,QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::DirectionS,QOpenGLTexture::Repeat);
    texture->setWrapMode(QOpenGLTexture::DirectionT,QOpenGLTexture::Repeat);

    if(texture==nullptr) qDebug()<<"texture is NULL";
    Texture tex;
    tex.path = path.toStdString();
    tex.id = texture->textureId();
    m_texturePropertys.push_back(tex);
    if(addtoProperty)
    {
        m_pPropertysWgt->addIcon(path, 0);
    }
    QApplication::processEvents(QEventLoop::AllEvents, 100);//防止阻塞界面
    return tex.id;
}

PropertyManager *PropertyManager::getPropertyManager()
{
    if(m_propertyManager == nullptr)
    {
        m_propertyManager = new PropertyManager();
    }
    return m_propertyManager;
}

Node *PropertyManager::copyNode(Node *node)
{
    if(node == nullptr)
    {
        return nullptr;
    }

    Node *retNode = nullptr;
    switch(node->type())
    {
    case NODE_TYPE_MODEL:
    {
        retNode = new Model();
    }
        break;
    case NODE_TYPE_MESH:
    {
        retNode = new Mesh(node->meshData(), node->textures());
    }
        break;
    default:
        break;
    }

    if(retNode)
    {
        retNode->setIdxInModel(node->IdxInModel());
        retNode->setName(node->name());
        retNode->setPath(node->path());
        retNode->setAABB(node->aabb());
        retNode->setScale(node->scale());
        retNode->setTranslate(node->translate());
        retNode->setRotate(node->rotate());
        //retNode->setHide(node->isHide());

        retNode->setMaterial(node->material());
        retNode->m_isAddAABBIdx = node->m_isAddAABBIdx;
        foreach(auto child, node->childs())
        {
            retNode->addChild(copyNode(child));
        }
    }

    return retNode;
}

Node *PropertyManager::loadNodeByData(QVariantMap data)
{
    QString path = data["path"].toString();
    QDir temDir(path);
    path = temDir.absolutePath();
    QFileInfo fileInfo(path);
    if(!fileInfo.isFile())
    {
        qDebug() << path << (" 加载失败!");
        return nullptr;
    }
    data["path"] = path;
    Node *retNode = nullptr;
    NODE_TYPE type = (NODE_TYPE)data["type"].toInt();
    switch(type){
    case NODE_TYPE_MODEL:
    {
        retNode = new Model();
    }
        break;
    case NODE_TYPE_MESH:
    {
        foreach(auto node, m_nodePropertys)
        {
            if(node->path() == path)
            {
                retNode = copyNode(node->childs()[data["idxInModel"].toInt()]);
            }
        }
        if(retNode == nullptr)
        {
            Node *tmpNode = new Model(path.toStdString().data());
            m_nodePropertys.push_back(tmpNode);
            retNode = copyNode(tmpNode->childs()[data["idxInModel"].toInt()]);
            m_pPropertysWgt->addIcon(path, 1);
        }
    }
        break;
    default:
        break;
    }

    if(retNode)
    {
        retNode->setData(data);
        QVariantList childs = data["childs"].toList();
        foreach(auto child, childs)
        {
            retNode->addChild(loadNodeByData(child.toMap()));
        }
    }
    return retNode;
}

void PropertyManager::addNode(Node *node, Node *parent)
{
    if(node == nullptr)
    {
        return;
    }
    m_isNeedSave = true;
    if(parent == nullptr)
    {
        m_nodes.append(node);
    }
    else
    {
        parent->addChild(node);
    }
    m_pNodesTreeWgt->addNode(node, parent);
}

void PropertyManager::delNode(Node *node)
{
    if(node == nullptr)
    {
        return;
    }
    if(node->parent() == nullptr)
    {
        m_nodes.removeOne(node);
    }
    else
    {
        node->parent()->delChild(node);
    }

    m_pNodeEditWgt->bindNode(nullptr);
    m_pNodesTreeWgt->delNode(node);
    m_pSelectNode = nullptr;
//    delete []node;
    m_isNeedSave = true;
}

void PropertyManager::selectNode(Node *node)
{
    m_pNodeEditWgt->bindNode(node);
    m_pNodesTreeWgt->selectNode(node);

    if(m_pSelectNode)
    {
        m_pSelectNode->setHideMesh(!m_isDrawMesh);
        m_pSelectNode->setHideAABB(true);
        m_pSelectNode->setHidelOutLine(true);
    }
    m_pSelectNode = node;
    if(m_pSelectNode)
    {
        m_pSelectNode->setHideMesh(!m_nodeSelectState.value(SHOWMESH).toBool());
        m_pSelectNode->setHideAABB(!m_nodeSelectState.value(SHOWAABB).toBool());
        m_pSelectNode->setHidelOutLine(!m_nodeSelectState.value(SHOWOUTLINE).toBool());
    }
    if(m_pManipulator == nullptr)
    {
        m_pManipulator = new ManipulatorTranslate();
    }
    m_pManipulator->bindModel(m_pSelectNode);
}

void PropertyManager::setIsDrawMesh(bool is)
{
    m_isDrawMesh = is;
    foreach(auto node, m_nodes)
    {
        node->setHideMesh(!is);
    }
    if(m_pSelectNode)
    {
        m_pSelectNode->setHideMesh(!m_nodeSelectState.value(SHOWMESH).toBool());
    }
}

void PropertyManager::setManipulatorType(MANIPULATOR_TYPE type)
{
    // return;
    switch (type) {
    case MANIPULATOR_TYPE::TRANSLATE:
    {
        m_pManipulator = m_pManipulator_t;
    }
        break;
    case MANIPULATOR_TYPE::ROTATE:
    {
        m_pManipulator = m_pManipulator_r;
    }
        break;
    case MANIPULATOR_TYPE::SCALE:
    {
        m_pManipulator = m_pManipulator_s;
    }
        break;
    default:
        break;
    }
    selectNode(m_pSelectNode);
}

void PropertyManager::setManipulatorReferenceSystem(MANIPULATOR_REFERENCE_SYSTEM referenceSystem)
{
    m_pManipulator->changeManipulatorReferenceSystem(referenceSystem);
}

void PropertyManager::removeAll()
{

}

void PropertyManager::addSelectTmpNode(float len, Node *node)
{
    if(node != nullptr)
    {
        m_selectTmpNodes[len] = node;
    }
}

void PropertyManager::openWorld(const QString &world)
{
    m_pUndoStack->clear();
    selectNode(nullptr);
    foreach(auto node, m_nodes)
    {
        delete []node;
    }
    m_nodes.clear();
    m_pNodesTreeWgt->removeAll();
    m_worldPath = world;
    QFile file(m_worldPath);
    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();
    file.close();

    // 解析
    QJsonDocument jsonDocu = QJsonDocument::fromJson(data);
    QJsonObject jsonObject = jsonDocu.object();

    QVariantMap dataMap = jsonObject.toVariantMap();

    QVariantList childs = dataMap["childs"].toList();
    if(!childs.isEmpty())
    {
        emit setEnabledSignal(false);
        //m_pLoadWgt->move((m_pLoadWgt->parentWidget()->width() - m_pLoadWgt->width())/2, (m_pLoadWgt->parentWidget()->height() - m_pLoadWgt->height())/2);
        m_pLoadWgt->show();
        m_pLoadWgt->onSetTip(("正在加载模型 0 %"));
        emit setTaskbarProgressValue(0);
        QFutureWatcher<void> *pWatcher = new QFutureWatcher<void>;
        QFuture<void> future = QtConcurrent::run([=]()
        {
            foreach(auto child, childs)
            {
                Node *pNode = loadNodeByData(child.toMap());
                if(pNode == nullptr)
                {
                    continue;
                }
                m_tmpNodes.append(pNode);
                int num = m_tmpNodes.size()* 100 / childs.size();
                g_PropertyManager->loadWgt()->onSetTip(QString(("正在加载模型 %1 %")).arg(num));
                emit setTaskbarProgressValue(num*0.3);
            }
        });

        connect(pWatcher, &QFutureWatcher<void>::finished,this,[=](){
            m_pLoadWgt->onSetTip(("正在加载纹理 0 %"));
            int idx = 0;
            foreach(auto node, m_nodePropertys)
            {
                node->setUpTextureId();
                idx++;
                int num = idx* 100 / m_nodePropertys.size();
                g_PropertyManager->loadWgt()->onSetTip(QString(("正在加载纹理 %1 %")).arg(num));
                emit setTaskbarProgressValue(30 + num*0.3);
            }
            foreach(auto node, m_tmpNodes)
            {
                node->setUpTextureId();
            }
            m_isLoadWorld = true;
        });
        pWatcher->setFuture(future);
    }
}

void PropertyManager::saveWorld(QString path)
{
    if(path == "")
    {
        path = m_worldPath;
    }
    if(path == "")
    {
        QFileDialog dialog;
        dialog.setFileMode(QFileDialog::AnyFile);
        path = dialog.getSaveFileName(nullptr,("场景保存"), g_Config->Get("path", "world").toString(), "world file (*.world)");
        m_worldPath = path;
        if(path == "")
        {
            return;
        }
    }
    QJsonDocument jsonDocu;

    QVariantMap data;
//    data.insert("light", "");
    QVariantList childs;
    foreach(auto child, m_nodes)
    {
        childs.append(child->saveData());
    }
    data.insert("childs", childs);

    QJsonObject json = QJsonObject::fromVariantMap(data);
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    jsonDocu.setObject(json);
    file.seek(0);
    file.write(jsonDocu.toJson());
    file.flush();
    file.close();
    m_isNeedSave = false;
}

PropertyManager::PropertyManager()
{
    m_pUndoStack = new QUndoStack;
    m_pPropertysWgt = new PropertysWgt();
    m_pNodesTreeWgt = new NodesTreeWgt();
    m_pNodeEditWgt  = new NodeEditWgt();
    m_pLoadWgt      = new LoadingWgt();

    m_pManipulator_t = new ManipulatorTranslate();
    m_pManipulator_r = new ManipulatorRotate();
    m_pManipulator_s = new ManipulatorScale();
    m_pManipulator = m_pManipulator_t;

    m_pCamera = new Camera();

    m_nodeSelectState.insert(SHOWAABB, g_Config->Get("nodeSelectState", SHOWAABB));
    m_nodeSelectState.insert(SHOWMESH, g_Config->Get("nodeSelectState", SHOWMESH));
    m_nodeSelectState.insert(SHOWOUTLINE, g_Config->Get("nodeSelectState", SHOWOUTLINE));
}

Node *PropertyManager::loadModel(const QString &path, bool addtoProperty)
{
    Node *retNode = nullptr;
    foreach(auto node, m_nodePropertys)
    {
        if(node->path() == path)
        {
            retNode = copyNode(node);
        }
    }
    if(retNode == nullptr)
    {
        Node *tmpNode = new Model(path.toStdString().data());
        m_nodePropertys.push_back(tmpNode);
        retNode = copyNode(tmpNode);
        if(addtoProperty)
        {
            m_pPropertysWgt->addIcon(path, 1);
        }
    }
    return retNode;
}
