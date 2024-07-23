#ifndef MESH_H
#define MESH_H
#include <QOpenGLShaderProgram>
#include <string>
#include <vector>
#include <QOpenGLTexture>
#include "node.h"
#include "PropertyManager/propertymanager.h"

class Mesh :public Node
{
public:
    void Draw(QOpenGLShaderProgram &shader, bool isDepthMap = false);
    Mesh(MeshData *meshData, vector<Texture> textures, Node *parent = nullptr);

    void setupGlFunc(GLFUNC *glfuns);

    void setUpTextureId();

    bool isInViewPort();
private:

};

#endif // MESH_H
