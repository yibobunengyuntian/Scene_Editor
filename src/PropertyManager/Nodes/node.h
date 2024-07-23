#ifndef NODE_H
#define NODE_H

#include <QVector3D>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_3_Compatibility>
//#include "../Components/componentbase.h"

#define GLFUNC QOpenGLFunctions_3_3_Compatibility

class PropertyManager;
class ComponentBase;
using namespace std;
struct Vertex {
    QVector3D Position;
    QVector3D Normal;
    QVector2D TexCoords;
};
struct Texture {
    unsigned int id;
    string type;
    string path;
};

struct AABB
{
    QVector3D minPos;
    QVector3D maxPos;
};

struct Material {
    float shininess;
    QVector3D ambient;
    QVector3D diffuse;
    QVector3D specular;
};
struct MeshData
{
    vector<Vertex> m_vertices;
    vector<unsigned int> m_indices;

    unsigned int VAO, VBO, EBO;

    bool m_isSetup = false;
};

enum NODE_TYPE
{
    NODE_TYPE_NONE  = -1,
    NODE_TYPE_MODEL =  0,
    NODE_TYPE_MESH  =  1,
};


class Node
{
public:
    Node(Node *parent = nullptr);
    virtual ~Node();
    QMatrix4x4 modelMat();
    void setAABB(const AABB &aabb){m_aabb = aabb;}
    AABB aabb(){return m_aabb;}
    void setParent(Node *parent){m_pParent = parent;}
    Node *parent(){return m_pParent;}
    QString path(){return m_path;}
    void setPath(const QString &path){m_path = path;}
    void setIdxInModel(int indx){m_nIdxInModel = indx;}
    int IdxInModel(){return m_nIdxInModel;}
    NODE_TYPE type(){return m_nodeType;}
    MeshData *meshData(){return m_pMeshData;}
    vector<Texture> textures(){return m_textures;}
    void setTextures(vector<Texture> textures){m_textures = textures;}
    QVariantMap saveData();
    void setData(QVariantMap data);

    void setHideAABB(bool isHide);
    bool isHideAABB(){ return m_isHideAABB; }
    bool isSelect(QVector4D pos);
    Node *selectNode(QVector4D pos);
    QVector3D centrePos(){return  m_centrePos;}

    QVector3D translate(){return  m_tra;}
    QVector3D rotate(){return  m_rot;}
    QVector3D scale(){return  m_scale;}

    QVector3D relativeScale();
    void setTranslate(const QVector3D &tra);
    void translate(const QVector3D &tra);

    void setRotate(const QVector3D& rot);
    void rotate(const QVector3D& rot);
    void rotateRectify();

    void setScale(const QVector3D& sca);
    void scale(const QVector3D& sca);
    void scaleMovePos(const QVector3D &sca, const QVector3D &pos);

    void setHide(bool is){m_isHide = is;}
    bool isHide(){return m_isHide;}
    void setHideMesh(bool is);
    void setHidelOutLine(bool is);
    void setMaterial(const Material &material);
    Material material(){return m_material;}
    void setName(const QString &name){m_name = name;}
    QString name(){return m_name;}
    void addChild(Node *node);
    QList<Node *> childs(){return m_childs;}
    void delChild(Node *child);
    void bindComponent(ComponentBase *com);

    virtual void Draw(QOpenGLShaderProgram &shader, bool isDepthMap = false);
    virtual void setupGlFunc(GLFUNC *glfuns);
    virtual void setUpTextureId();

    bool m_isAddAABBIdx = false;
protected:
    virtual void upDataAABB();


protected:
    GLFUNC *m_glFuns;
    // mesh data
    MeshData *m_pMeshData;
    vector<Texture> m_textures;

    QString m_name;
    QString m_path;
    int m_nIdxInModel = -1;
    QMatrix4x4 m_modelMat;
    QVector3D m_tra = QVector3D(0, 0, 0);
    QVector3D m_rot = QVector3D(0, 0, 0);   //分别为绕x y z轴旋转的角度;
    QVector3D m_scale = QVector3D(1, 1, 1);

    Node *m_pParent = nullptr;
    QList<Node *> m_childs;
    AABB m_aabb;
    bool m_isHideAABB = true;
    QVector3D m_centrePos;
    bool m_isHideMesh = true;
    bool m_isHideOutLine = true;
    bool m_isHide = false;
    bool m_isUseColor = false;
    NODE_TYPE m_nodeType = NODE_TYPE::NODE_TYPE_NONE;
    QVector3D m_color = QVector3D(1, 1, 1);
    Material m_material{32.0f, QVector3D(0.4f, 0.4f, 0.4f), QVector3D(0.9f, 0.9f, 0.9f),QVector3D(0.3f, 0.3f, 0.3f)};
    ComponentBase *m_pComponent = nullptr;
    //...
};

#endif // NODE_H
