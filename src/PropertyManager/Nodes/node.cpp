#include "node.h"
#include "./../propertymanager.h"

Node::Node(Node *parent)
    :m_pParent(parent)
{

}

Node::~Node()
{
    foreach(auto child, m_childs)
    {
        delete []child;
    }
    if(m_pComponent)
    {
        delete m_pComponent;
        m_pComponent = nullptr;
    }
}

QMatrix4x4 Node::modelMat()
{
    QMatrix4x4 modelMat = QMatrix4x4();
    if(parent())
    {
        QVector3D tra, sca;
        tra = m_tra - parent()->translate();
        sca = m_scale;///parent()->scale();

        modelMat.translate(tra);

        modelMat.rotate(m_rot.y(), 0, 1, 0);
        modelMat.rotate(m_rot.z(), 0, 0, 1);
        modelMat.rotate(m_rot.x(), 1, 0, 0);

        modelMat.scale(sca);
        QMatrix4x4 pMat = parent()->modelMat();
        pMat.scale(QVector3D(1, 1, 1)/parent()->scale());
        modelMat = pMat * modelMat;
    }
    else
    {
        modelMat.translate(m_tra);

        modelMat.rotate(m_rot.y(), 0, 1, 0);
        modelMat.rotate(m_rot.z(), 0, 0, 1);
        modelMat.rotate(m_rot.x(), 1, 0, 0);

        modelMat.scale(m_scale);
    }

    return modelMat;
}

QVariantMap Node::saveData()
{
    QVariantMap data;
    data.insert("name", m_name);
    data.insert("type", m_nodeType);

    //获取程序所在的路径
    QDir dir=QDir::current();
        //将绝对路径转换为相对路径
    QString relPath = dir.relativeFilePath(m_path);
    data.insert("path", relPath);
    data.insert("idxInModel", m_nIdxInModel);
    data.insert("isHide", m_isHide);
    QString tra = QString::number(m_tra.x()) + "," + QString::number(m_tra.y()) + "," + QString::number(m_tra.z());
    QString rot = QString::number(m_rot.x()) + "," + QString::number(m_rot.y()) + "," + QString::number(m_rot.z());
    QString sca = QString::number(m_scale.x()) + "," + QString::number(m_scale.y()) + "," + QString::number(m_scale.z());
    data.insert("tra", tra);
    data.insert("rot", rot);
    data.insert("sca", sca);
    QString mat_am = QString::number(m_material.ambient.x()) + "," + QString::number(m_material.ambient.y()) + "," + QString::number(m_material.ambient.z());
    QString mat_di = QString::number(m_material.diffuse.x()) + "," + QString::number(m_material.diffuse.y()) + "," + QString::number(m_material.diffuse.z());
    QString mat_sp = QString::number(m_material.specular.x()) + "," + QString::number(m_material.specular.y()) + "," + QString::number(m_material.specular.z());
    data.insert("mat_sh", m_material.shininess);
    data.insert("mat_am", mat_am);
    data.insert("mat_di", mat_di);
    data.insert("mat_sp", mat_sp);
    data.insert("isUseClolr", m_isUseColor);
    QString color = QString::number(m_color.x()) + "," + QString::number(m_color.y()) + "," + QString::number(m_color.z());
    data.insert("color", color);
    QVariantList childs;
    foreach(auto child, m_childs)
    {
        childs.append(child->saveData());
    }
    data.insert("childs", childs);
    return data;
}

void Node::setData(QVariantMap data)
{
    m_name = data["name"].toString();
    m_nodeType = (NODE_TYPE)data["type"].toInt();
    m_path = data["path"].toString();
    m_nIdxInModel = data["idxInModel"].toInt();
    m_isHide = data["isHide"].toBool();
    QStringList tra = data["tra"].toString().split(",");
    m_tra = QVector3D(tra[0].toDouble(), tra[1].toDouble(), tra[2].toDouble());
    QStringList rot = data["rot"].toString().split(",");
    m_rot = QVector3D(rot[0].toDouble(), rot[1].toDouble(), rot[2].toDouble());
    QStringList sca = data["sca"].toString().split(",");
    m_scale = QVector3D(sca[0].toDouble(), sca[1].toDouble(), sca[2].toDouble());
    m_material.shininess = data["mat_sh"].toFloat();
    QStringList mat_am = data["mat_am"].toString().split(",");
    m_material.ambient = QVector3D(mat_am[0].toDouble(), mat_am[1].toDouble(), mat_am[2].toDouble());
    QStringList mat_di = data["mat_di"].toString().split(",");
    m_material.diffuse = QVector3D(mat_di[0].toDouble(), mat_di[1].toDouble(), mat_di[2].toDouble());
    QStringList mat_sp = data["mat_sp"].toString().split(",");
    m_material.specular = QVector3D(mat_sp[0].toDouble(), mat_sp[1].toDouble(), mat_sp[2].toDouble());
    m_isUseColor = data["isUseCloor"].toBool();
    QStringList color = data["color"].toString().split(",");
    m_color = QVector3D(color[0].toDouble(), color[1].toDouble(), color[2].toDouble());
}

void Node::setHideAABB(bool isHide)
{
    m_isHideAABB = isHide;
    foreach (auto node, m_childs)
    {
       node->setHideAABB(m_isHideAABB);
    }
}

bool Node::isSelect(QVector4D pos)
{
    if(m_isHide)
    {
        return false;
    }
    QVector3D minPos = modelMat() * m_aabb.minPos;
    QVector3D maxPos = modelMat() * m_aabb.maxPos;
    // if(m_rot != QVector3D())
    {
        QVector<QVector3D> ps{modelMat() * QVector3D(m_aabb.minPos.x(), m_aabb.minPos.y(), m_aabb.maxPos.z()),
                              modelMat() * QVector3D(m_aabb.minPos.x(), m_aabb.maxPos.y(), m_aabb.maxPos.z()),
                              modelMat() * QVector3D(m_aabb.maxPos.x(), m_aabb.minPos.y(), m_aabb.maxPos.z()),
                              modelMat() * QVector3D(m_aabb.maxPos.x(), m_aabb.minPos.y(), m_aabb.minPos.z()),
                              modelMat() * QVector3D(m_aabb.minPos.x(), m_aabb.minPos.y(), m_aabb.maxPos.z()),
                              modelMat() * QVector3D(m_aabb.minPos.x(), m_aabb.maxPos.y(), m_aabb.minPos.z())};

        foreach(auto pos, ps)
        {
            if(pos.x() < minPos.x())
            {
                minPos.setX(pos.x());
            }
            if(pos.y() < minPos.y())
            {
                minPos.setY(pos.y());
            }
            if(pos.z() < minPos.z())
            {
                minPos.setZ((pos.z()));
            }

            if(pos.x() > maxPos.x())
            {
                maxPos.setX(pos.x());
            }
            if(pos.y() > maxPos.y())
            {
                maxPos.setY(pos.y());
            }
            if(pos.z() > maxPos.z())
            {
                maxPos.setZ((pos.z()));
            }
        }
    }
    if(pos.x() >= minPos.x()-0.01f && pos.y() >= minPos.y()-0.01f && pos.z() >= minPos.z()-0.01f
        && pos.x() <= maxPos.x()+0.01f && pos.y() <= maxPos.y()+0.01f && pos.z() <= maxPos.z()+0.01f)
    {
        g_PropertyManager->addSelectTmpNode(QVector3D(maxPos - minPos).length(), this);
        return true;
    }
    return false;
}

Node *Node::selectNode(QVector4D pos)
{
    if(m_isHide)
    {
        return nullptr;
    }
    Node *retNode = nullptr;
    if(m_nodeType == NODE_TYPE::NODE_TYPE_MODEL)
    {
        foreach(auto node, m_childs)
        {
            retNode = node->selectNode(pos);
            if(retNode != nullptr)
            {
                //return retNode;
            }
        }
    }
    if(m_nodeType == NODE_TYPE::NODE_TYPE_MESH)
    {
        if(isSelect(pos))
        {
            //return this;
        }
        foreach(auto node, m_childs)
        {
            retNode = node->selectNode(pos);
            if(retNode != nullptr)
            {
                //return retNode;
            }
        }
    }
    return nullptr;
}

QVector3D Node::relativeScale()
{
    if(parent())
    {
        return m_scale/parent()->relativeScale();
    }
    else
    {
        return m_scale;
    }
}

void Node::setTranslate(const QVector3D &tra)
{
    if(isnan(tra.x()) || isnan(tra.y()) || isnan(tra.z()))
    {
        return;
    }
    foreach(auto node, m_childs)
    {
        node->translate(tra - m_tra);
    }
    m_tra = tra;
}

void Node::translate(const QVector3D &tra)
{
    if(isnan(tra.x()) || isnan(tra.y()) || isnan(tra.z()))
    {
        return;
    }
    foreach(auto node, m_childs)
    {
        node->translate(tra);
    }
    m_tra += tra;
}

void Node::setRotate(const QVector3D &rot)
{
    if(isnan(rot.x()) || isnan(rot.y()) || isnan(rot.z()))
    {
        return;
    }

    // foreach(auto node, m_childs)
    // {
    //     node->rotate(rot - m_rot);
    // }
    m_rot = rot;
    rotateRectify();
}

void Node::rotate(const QVector3D &rot)
{
    if(isnan(rot.x()) || isnan(rot.y()) || isnan(rot.z()))
    {
        return;
    }
    m_rot += rot;
    // foreach(auto child, m_childs)
    // {
    //     child->rotate(rot);
    // }
    rotateRectify();
}

void Node::rotateRectify()
{
    if(m_rot.x() < 0) m_rot.setX(m_rot.x() + 360);
    if(m_rot.x() > 360) m_rot.setX(m_rot.x() - 360);
    if(m_rot.y() < 0) m_rot.setY(m_rot.y() + 360);
    if(m_rot.y() > 360) m_rot.setY(m_rot.y() - 360);
    if(m_rot.z() < 0) m_rot.setZ(m_rot.z() + 360);
    if(m_rot.z() > 360) m_rot.setZ(m_rot.z() - 360);
}

void Node::setScale(const QVector3D &sca)
{
    if(isnan(sca.x()) || isnan(sca.y()) || isnan(sca.z()))
    {
        return;
    }
    QVector3D tmpSca = QVector3D(1, 1, 1);
    if(m_scale.x() != sca.x())
    {
        tmpSca.setX(sca.x()/m_scale.x());
    }
    if(m_scale.y() != sca.y())
    {
        tmpSca.setY(sca.y()/m_scale.y());
    }
    if(m_scale.z() != sca.z())
    {
        tmpSca.setZ(sca.z()/m_scale.z());
    }
    m_scale = sca;
    foreach(auto node, m_childs)
    {
        QMatrix4x4 mat;
//        QVector3D rot = node->rotate();
//        mat.rotate(rot.y(), 0, 1, 0);
//        mat.rotate(rot.z(), 0, 0, 1);
//        mat.rotate(rot.x(), 1, 0, 0);
        node->scaleMovePos(mat*tmpSca, m_tra);
    }
}

void Node::scale(const QVector3D &sca)
{
    if(isnan(sca.x()) || isnan(sca.y()) || isnan(sca.z()))
    {
        return;
    }
    m_scale *= sca;
    foreach(auto node, m_childs)
    {
        QMatrix4x4 mat;
//        QVector3D rot = node->rotate();
//        mat.rotate(rot.y(), 0, 1, 0);
//        mat.rotate(rot.z(), 0, 0, 1);
//        mat.rotate(rot.x(), 1, 0, 0);
        node->scaleMovePos(mat*sca, m_tra);
    }
}

void Node::scaleMovePos(const QVector3D &sca, const QVector3D &pos)
{
    if(isnan(sca.x()) || isnan(sca.y()) || isnan(sca.z()))
    {
        return;
    }
    m_scale *= sca;
    m_tra = (m_tra - pos)*sca + pos;
    foreach(auto node, m_childs)
    {
        node->scaleMovePos(sca, m_tra);
    }
}

void Node::setHideMesh(bool is)
{
    m_isHideMesh = is;
    foreach (auto node, m_childs)
    {
        node->setHideMesh(m_isHideMesh);
    }
}

void Node::setHidelOutLine(bool is)
{
    m_isHideOutLine = is;
    foreach(auto node, m_childs)
    {
        node->setHidelOutLine(m_isHideOutLine);
    }
}

void Node::setMaterial(const Material &material)
{
    m_material = material;
    if(m_nodeType == NODE_TYPE::NODE_TYPE_MODEL)
    {
//        foreach(auto child, m_childs)
//        {
//            child->setMaterial(m_material);
//        }
    }
}

void Node::addChild(Node *node)
{
    if(node != nullptr)
    {
        node->setParent(this);
        m_childs.push_back(node);
    }
}

void Node::delChild(Node *child)
{
    m_childs.removeOne(child);
}

void Node::bindComponent(ComponentBase *com)
{
    if(m_pComponent)
    {
        delete []m_pComponent;
    }
    m_pComponent = com;
    if(m_pComponent)
    {
        m_pComponent->bindNode(this);
    }
}

void Node::upDataAABB()
{
//    m_aabb.minPos = QVector3D(m_modelMat * m_vertices[0].pos);
//    m_aabb.maxPos = QVector3D(m_modelMat * m_vertices[0].pos);
//    for(int i = 0; i < m_vertices.size(); i++)
//    {
//        QVector3D tmp = m_modelMat * m_vertices[i].pos;
//        if(tmp.x() > m_aabb.maxPos.x())
//        {
//            m_aabb.maxPos.setX(tmp.x());
//        }
//        if(tmp.y() > m_aabb.maxPos.y())
//        {
//            m_aabb.maxPos.setY(tmp.y());
//        }
//        if(tmp.z() > m_aabb.maxPos.z())
//        {
//            m_aabb.maxPos.setZ(tmp.z());
//        }

//        if(tmp.x() < m_aabb.minPos.x())
//        {
//            m_aabb.minPos.setX(tmp.x());
//        }
//        if(tmp.y() < m_aabb.minPos.y())
//        {
//            m_aabb.minPos.setY(tmp.y());
//        }
//        if(tmp.z() < m_aabb.minPos.z())
//        {
//            m_aabb.minPos.setZ(tmp.z());
//        }
//        //vertices.push_back(QVector3D(m_vertices[i], m_vertices[i+1], m_vertices[i+2]));
//    }

//    m_aabb.minPos.setX(m_aabb.minPos.x() - 0.01f);
//    m_aabb.minPos.setY(m_aabb.minPos.y() - 0.01f);
//    m_aabb.minPos.setZ(m_aabb.minPos.z() - 0.01f);
//    m_aabb.maxPos.setX(m_aabb.maxPos.x() + 0.01f);
//    m_aabb.maxPos.setY(m_aabb.maxPos.y() + 0.01f);
//    m_aabb.maxPos.setZ(m_aabb.maxPos.z() + 0.01f);
//    m_centrePos = (m_aabb.minPos +m_aabb.maxPos)/2;

//    m_AABBvertices.clear();
//    m_AABBvertices.push_back(QVector3D(m_aabb.minPos.x(), m_aabb.minPos.y(), m_aabb.minPos.z()));
//    m_AABBvertices.push_back(QVector3D(m_aabb.maxPos.x(), m_aabb.minPos.y(), m_aabb.minPos.z()));

//    m_AABBvertices.push_back(QVector3D(m_aabb.minPos.x(), m_aabb.maxPos.y(), m_aabb.minPos.z()));

//    m_AABBvertices.push_back(QVector3D(m_aabb.minPos.x(), m_aabb.minPos.y(), m_aabb.maxPos.z()));

//    m_AABBvertices.push_back(QVector3D(m_aabb.maxPos.x(), m_aabb.maxPos.y(), m_aabb.minPos.z()));

//    m_AABBvertices.push_back(QVector3D(m_aabb.maxPos.x(), m_aabb.maxPos.y(), m_aabb.maxPos.z()));

//    m_AABBvertices.push_back(QVector3D(m_aabb.maxPos.x(), m_aabb.minPos.y(), m_aabb.maxPos.z()));

    //    m_AABBvertices.push_back(QVector3D(m_aabb.minPos.x(), m_aabb.maxPos.y(), m_aabb.maxPos.z()));
}

void Node::Draw(QOpenGLShaderProgram &shader, bool isDepthMap)
{
    Q_UNUSED(shader)
    Q_UNUSED(isDepthMap)
}

void Node::setupGlFunc(GLFUNC *glfuns)
{
    Q_UNUSED(glfuns)
}

void Node::setUpTextureId()
{

}
