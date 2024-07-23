#include "nodeeditwgt.h"
#include "ui_nodeeditwgt.h"
#include "propertymanager.h"

NodeEditWgt::NodeEditWgt(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    QList<QLineEdit*> lineEdits = findChildren<QLineEdit*>();
    for(int i = 0; i < lineEdits.size(); i++)
    {
        QLineEdit* target = lineEdits.at(i);
        target->setContextMenuPolicy(Qt::NoContextMenu);
    }
    QList<QDoubleSpinBox*> doubleSpinBoxs = findChildren<QDoubleSpinBox*>();
    for(int i = 0; i < doubleSpinBoxs.size(); i++)
    {
        QDoubleSpinBox* target = doubleSpinBoxs.at(i);
        target->setContextMenuPolicy(Qt::NoContextMenu);
    }

    scrollAreaWgt->hide();

    btn_d->setStyleSheet(m_styleSheet);
    btn_d->setToolButtonStyle(Qt::ToolButtonIconOnly);
    btn_d->setIcon(QIcon(":/res/icon/open.png"));
    btn_s->setStyleSheet(m_styleSheet);
    btn_s->setToolButtonStyle(Qt::ToolButtonIconOnly);
    btn_s->setIcon(QIcon(":/res/icon/open.png"));
    btn_n->setStyleSheet(m_styleSheet);
    btn_n->setToolButtonStyle(Qt::ToolButtonIconOnly);
    btn_n->setIcon(QIcon(":/res/icon/open.png"));

    QDoubleSpinBox *SpinBox = matSpinBox_ns;
    connect(matSlider_ns, &QSlider::valueChanged, this, [SpinBox](int value){
        SpinBox->setValue(value/100.0);
    });
    connect(tra_x, SIGNAL(valueChanged(double)), this, SLOT(onTraChanged()));
    connect(tra_y, SIGNAL(valueChanged(double)), this, SLOT(onTraChanged()));
    connect(tra_z, SIGNAL(valueChanged(double)), this, SLOT(onTraChanged()));

    connect(rot_x, SIGNAL(valueChanged(double)), this, SLOT(onRotChanged()));
    connect(rot_y, SIGNAL(valueChanged(double)), this, SLOT(onRotChanged()));
    connect(rot_z, SIGNAL(valueChanged(double)), this, SLOT(onRotChanged()));

    connect(sca_x, SIGNAL(valueChanged(double)), this, SLOT(onScaChanged()));
    connect(sca_y, SIGNAL(valueChanged(double)), this, SLOT(onScaChanged()));
    connect(sca_z, SIGNAL(valueChanged(double)), this, SLOT(onScaChanged()));

    connect(matSpinBox_ns,   SIGNAL(valueChanged(double)), this, SLOT(onMaterialChanged()));
    connect(matSpinBox_ka_r, SIGNAL(valueChanged(double)), this, SLOT(onMaterialChanged()));
    connect(matSpinBox_ka_g, SIGNAL(valueChanged(double)), this, SLOT(onMaterialChanged()));
    connect(matSpinBox_ka_b, SIGNAL(valueChanged(double)), this, SLOT(onMaterialChanged()));
    connect(matSpinBox_ks_r, SIGNAL(valueChanged(double)), this, SLOT(onMaterialChanged()));
    connect(matSpinBox_ks_g, SIGNAL(valueChanged(double)), this, SLOT(onMaterialChanged()));
    connect(matSpinBox_ks_b, SIGNAL(valueChanged(double)), this, SLOT(onMaterialChanged()));
    connect(matSpinBox_kd_r, SIGNAL(valueChanged(double)), this, SLOT(onMaterialChanged()));
    connect(matSpinBox_kd_g, SIGNAL(valueChanged(double)), this, SLOT(onMaterialChanged()));
    connect(matSpinBox_kd_b, SIGNAL(valueChanged(double)), this, SLOT(onMaterialChanged()));
}

void NodeEditWgt::bindNode(Node *node)
{
    if(m_pBindNode == node)
    {
        return;
    }
    m_pBindNode = node;
    update();
}

void NodeEditWgt::onMaterialChanged()
{
    if(m_isUpdate)
    {
        return;
    }
    if(m_pBindNode)
    {
        Material mat = m_pBindNode->material();
        mat.shininess = static_cast<float>(matSpinBox_ns->value());
        mat.ambient = QVector3D(matSpinBox_ka_r->value(), matSpinBox_ka_g->value(), matSpinBox_ka_b->value());
        mat.diffuse = QVector3D(matSpinBox_kd_r->value(), matSpinBox_kd_g->value(), matSpinBox_kd_b->value());
        mat.specular = QVector3D(matSpinBox_ks_r->value(), matSpinBox_ks_g->value(), matSpinBox_ks_b->value());
//        m_pBindNode->setMaterial(mat);
        g_PropertyManager->undoStack()->push(new UndoCommandChangeNodeMaterial(m_pBindNode, mat, "Change Node Material"));

        g_PropertyManager->setIsNeedSave(true);
    }
}

void NodeEditWgt::onNameEditChanaged(const QString &text)
{
    if(m_isUpdate)
    {
        return;
    }
    if(m_pBindNode)
    {
        m_pBindNode->setName(text);
    }
}

void NodeEditWgt::onTraChanged()
{
    if(m_isUpdate)
    {
        return;
    }
    if(m_pBindNode)
    {
//        m_pBindNode->setTranslate(QVector3D(tra_x->value(), tra_y->value(), tra_z->value()));
        g_PropertyManager->undoStack()->push(new UndoCommandTraNode(m_pBindNode,
                                                                    QVector3D(tra_x->value(), tra_y->value(), tra_z->value()) - m_pBindNode->translate(), "translate node"));
        g_PropertyManager->setIsNeedSave(true);
    }
}

void NodeEditWgt::onRotChanged()
{
    if(m_isUpdate)
    {
        return;
    }
    if(rot_x->value() == 360)
    {
        rot_x->setValue(0);
    }
    if(rot_y->value() == 360)
    {
        rot_y->setValue(0);
    }
    if(rot_z->value() == 360)
    {
        rot_z->setValue(0);
    }
    if(m_pBindNode)
    {
//        m_pBindNode->setRotate(QVector3D(rot_x->value(), rot_y->value(), rot_z->value()));
        g_PropertyManager->undoStack()->push(new UndoCommandRotNode(m_pBindNode,
                                                                    QVector3D(rot_x->value(), rot_y->value(), rot_z->value()) - m_pBindNode->rotate(), "translate node"));
        g_PropertyManager->setIsNeedSave(true);
    }
}

void NodeEditWgt::onScaChanged()
{
    if(m_isUpdate)
    {
        return;
    }
    if(m_pBindNode)
    {
//        m_pBindNode->setScale(QVector3D(sca_x->value(), sca_y->value(), sca_z->value()));
        g_PropertyManager->undoStack()->push(new UndoCommandScaNode(m_pBindNode,
                                                                    QVector3D(sca_x->value(), sca_y->value(), sca_z->value())/m_pBindNode->scale(), "translate node"));
        g_PropertyManager->setIsNeedSave(true);
    }
}

void NodeEditWgt::onTextureChanged()
{
    return;
    if(m_isUpdate)
    {
        return;
    }
    LineEdit *ple = qobject_cast<LineEdit *>(sender());
    if(ple == nullptr) return;
    QString path = ple->text();
    bool fileIsnull = true;
    QFileInfo fileInfo(path);
    if(!fileInfo.isFile())
    {
        fileIsnull = true;
        ple->setStyleSheet("background-color: rgb(118, 118, 118);\
                            border: 0px;\
                            color: rgb(233, 0, 0);");
    }
    else
    {
        fileIsnull = false;
        ple->setStyleSheet("background-color: rgb(118, 118, 118);\
                            border: 0px;\
                            color: rgb(255, 255, 255);");
    }
    vector<Texture> textures = m_pBindNode->textures();
    bool isChange = false;
    for(auto it = textures.begin(); it != textures.end();)
    {
        if(ple->objectName().toStdString() == it->type)
        {
            isChange = true;
            if(fileIsnull)
            {
                it = textures.erase(it);
            }
            else
            {
                it->path == path.toStdString();
                it++;
            }
        }
    }
    if(!isChange)
    {
        Texture tex;
        tex.path = path.toStdString();
        tex.type = ple->objectName().toStdString();
        textures.push_back(tex);
    }
    m_pBindNode->setTextures(textures);
    m_pBindNode->setUpTextureId();
}

void NodeEditWgt::update()
{
    if(m_pBindNode == nullptr)
    {
        scrollAreaWgt->hide();
        return;
    }
    m_isUpdate = true;
    box_m->setHidden(m_pBindNode->type() == NODE_TYPE_MODEL);
    box_t->setHidden(m_pBindNode->type() == NODE_TYPE_MODEL);

    QVector3D tra = m_pBindNode->translate();
    QVector3D rot = m_pBindNode->rotate();
    QVector3D sca = m_pBindNode->scale();
    tra_x->setValue(tra.x());
    tra_y->setValue(tra.y());
    tra_z->setValue(tra.z());
    rot_x->setValue(rot.x());
    rot_y->setValue(rot.y());
    rot_z->setValue(rot.z());
    sca_x->setValue(sca.x());
    sca_y->setValue(sca.y());
    sca_z->setValue(sca.z());
    Material mat = m_pBindNode->material();
    matSlider_ns->setValue(static_cast<int>(mat.shininess * 100));

    matSpinBox_ka_r->setValue(mat.ambient.x());
    matSpinBox_ka_g->setValue(mat.ambient.y());
    matSpinBox_ka_b->setValue(mat.ambient.z());

    matSpinBox_kd_r->setValue(mat.diffuse.x());
    matSpinBox_kd_g->setValue(mat.diffuse.y());
    matSpinBox_kd_b->setValue(mat.diffuse.z());

    matSpinBox_ks_r->setValue(mat.specular.x());
    matSpinBox_ks_g->setValue(mat.specular.y());
    matSpinBox_ks_b->setValue(mat.specular.z());
    texture_diffuse->clear();
    texture_specular->clear();
    texture_normals->clear();
    vector<Texture> textures = m_pBindNode->textures();
    for(unsigned int i = 0; i < textures.size(); i++) {
        string type = textures[i].type;
        if(type == "texture_diffuse")
        {
            texture_diffuse->setText(QString(textures[i].path.data()));
        }
        else if(type == "texture_specular")
        {
            texture_specular->setText(QString(textures[i].path.data()));
        }
        else if(type == "texture_normals")
        {
            texture_normals->setText(QString(textures[i].path.data()));
        }
    }

    scrollAreaWgt->show();
    m_isUpdate = false;
}
