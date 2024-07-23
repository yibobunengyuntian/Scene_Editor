#ifndef MODEL_H
#define MODEL_H
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <QDir>
#include "mesh.h"
#include "node.h"
class Mesh;
class Model: public Node
{
public:
    vector<Texture> textures_loaded;
    Model(const char *path){
        m_nodeType = NODE_TYPE::NODE_TYPE_MODEL;
        m_path = path;
        loadModel(path);
    }
    Model()
    {
        m_nodeType = NODE_TYPE::NODE_TYPE_MODEL;
    }

    void setupGlFunc(GLFUNC *glfuns){
        for(int i = 0; i < m_childs.size(); i++)
            m_childs[i]->setupGlFunc(glfuns);
    }

    void Draw(QOpenGLShaderProgram &shader, bool isDepthMap = false) {
        if(m_isHide)
        {
            return;
        }
        for(int i = 0; i < m_childs.size(); i++)
            m_childs[i]->Draw(shader, isDepthMap);
    }

    void setUpTextureId(){
        for(int i = 0; i < m_childs.size(); i++)
            m_childs[i]->setUpTextureId();
    }
    QString findFile(QString filepath, QString name);

protected:
    void upDataAABB();

private:
    //vector<Mesh> meshes;
    string directory;
    void loadModel(string path);

    void processNode(aiNode *node, const aiScene *scene);
    Mesh* processMesh(aiMesh *mesh, const aiScene *scene);
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
    unsigned int TextureFromFile(const char *path, const string &directory);
};

#endif // MODEL_H
