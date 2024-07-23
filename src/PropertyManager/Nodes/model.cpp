#include "model.h"

void Model::upDataAABB()
{

}

void Model::loadModel(string path)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate |
                                           aiProcess_JoinIdenticalVertices |
                                           aiProcess_GenSmoothNormals);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        qDebug() << "ERROR::ASSIMP::" << path.data() << "::" << import.GetErrorString();
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);

    path = path.substr(0, path.find_last_of('.'));
    QFileInfo file(QString::fromStdString(path));
    m_name = file.fileName();
    if(m_childs.count() == 0)
    {
        return;
    }
    int n = 0;
    m_aabb = m_childs[0]->aabb();
    foreach(auto child, m_childs)
    {
        child->setPath(m_path);
        child->setIdxInModel(n++);
        AABB aabb = child->aabb();
        if(m_aabb.minPos.x() > aabb.minPos.x())
        {
            m_aabb.minPos.setX(aabb.minPos.x());
        }
        if(m_aabb.minPos.y() > aabb.minPos.y())
        {
            m_aabb.minPos.setY(aabb.minPos.y());
        }
        if(m_aabb.minPos.z() > aabb.minPos.z())
        {
            m_aabb.minPos.setZ(aabb.minPos.z());
        }

        if(m_aabb.maxPos.x() < aabb.maxPos.x())
        {
            m_aabb.maxPos.setX(aabb.maxPos.x());
        }
        if(m_aabb.maxPos.y() < aabb.maxPos.y())
        {
            m_aabb.maxPos.setY(aabb.maxPos.y());
        }
        if(m_aabb.maxPos.z() < aabb.maxPos.z())
        {
            m_aabb.maxPos.setZ(aabb.maxPos.z());
        }
    }
    QVector3D pos = (m_aabb.maxPos + m_aabb.minPos)/2;
    m_aabb.maxPos -= pos;
    m_aabb.minPos -= pos;
    m_tra = pos;
    if((m_aabb.maxPos - m_aabb.minPos).length() > 1000)
    {
        foreach(auto child, m_childs)
        {
            child->scaleMovePos(QVector3D(0.001f, 0.001f, 0.001f), m_tra);
        }
    }
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    // process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        m_childs.push_back(processMesh(mesh, scene));
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh* Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    AABB aabb;
    QVector3D pos = QVector3D(0, 0, 0);
    if(mesh->mNumVertices > 0)
    {
        aabb.minPos = QVector3D(mesh->mVertices[0].x, mesh->mVertices[0].y, mesh->mVertices[0].z);
        aabb.maxPos = QVector3D(mesh->mVertices[0].x, mesh->mVertices[0].y, mesh->mVertices[0].z);
    }
    Vertex vertex;
    aiVector3D aivector;
    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        aivector = mesh->mVertices[i];
        vertex.Position = QVector3D(aivector.x, aivector.y, aivector.z);
        if(aivector.x < aabb.minPos.x())
        {
            aabb.minPos.setX(aivector.x);
        }
        if(aivector.y < aabb.minPos.y())
        {
            aabb.minPos.setY(aivector.y);
        }
        if(aivector.z < aabb.minPos.z())
        {
            aabb.minPos.setZ(aivector.z);
        }

        if(aivector.x > aabb.maxPos.x())
        {
            aabb.maxPos.setX(aivector.x);
        }
        if(aivector.y > aabb.maxPos.y())
        {
            aabb.maxPos.setY(aivector.y);
        }
        if(aivector.z > aabb.maxPos.z())
        {
            aabb.maxPos.setZ(aivector.z);
        }

        aivector = mesh->mNormals[i];
        vertex.Normal = QVector3D(aivector.x, aivector.y, aivector.z);

        if(mesh->mTextureCoords[0])
        {
            QVector2D vec;
            vec.setX(mesh->mTextureCoords[0][i].x);
            vec.setY(mesh->mTextureCoords[0][i].y);
            vertex.TexCoords = vec;
        } else
            vertex.TexCoords = QVector2D(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    pos = (aabb.maxPos + aabb.minPos)/2;
    for(vector<Vertex>::iterator it = vertices.begin(); it != vertices.end(); ++it)
    {
        it->Position -= pos;
    }
    aabb.maxPos -= pos;
    aabb.minPos -= pos;
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    Material mat;
    QString name;
    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        vector<Texture> diffuseMaps =
                loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        vector<Texture> specularMaps =
                loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        vector<Texture> normalsMaps =
            loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normals");
        textures.insert(textures.end(), normalsMaps.begin(), normalsMaps.end());
        aiColor3D tmp;
        material->Get(AI_MATKEY_COLOR_AMBIENT, tmp);
        mat.ambient = QVector3D(tmp.r, tmp.g, tmp.b);

        material->Get(AI_MATKEY_COLOR_DIFFUSE, tmp);
        mat.diffuse = QVector3D(tmp.r, tmp.g, tmp.b);

        material->Get(AI_MATKEY_COLOR_SPECULAR, tmp);
        mat.specular = QVector3D(tmp.r, tmp.g, tmp.b);

        float sh;
        material->Get(AI_MATKEY_SHININESS, sh);
        if(sh < 0.01f)
        {
            sh = 32;
        }
        mat.shininess = sh;

        aiString str =  mesh->mName;
        name = QString::fromLocal8Bit(str.data);
    }
    MeshData *meshData = new MeshData;
    meshData->m_indices = indices;
    meshData->m_vertices = vertices;
    Mesh *newMesh = new Mesh(meshData, textures);
    newMesh->setParent(this);
    newMesh->setMaterial(mat);
    newMesh->setAABB(aabb);
    newMesh->setTranslate(pos);

    newMesh->setName(name);
    return newMesh;
}

vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
{
    vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for(unsigned int j = 0; j < textures_loaded.size(); j++) {
            if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if(!skip){
            Texture texture;
            //texture.id = TextureFromFile(str.C_Str(), directory);
            texture.type = typeName;

            string filename = string(str.C_Str());
            QString name(filename.data());
            QStringList namelist = name.split("/");
            QFileInfo file(name);

            filename = findFile(QString(directory.data()), file.fileName()).toStdString();

            texture.path = filename;
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}

unsigned int Model::TextureFromFile(const char *path, const string &directory)
{
    string filename = string(path);
    QString name(filename.data());
    QStringList namelist = name.split("/");

    if(namelist.size() < 2)
    {
        namelist = name.split("\\");
    }
    if(namelist.size() > 1)
    {
        filename = namelist[namelist.size() - 2].toStdString() + "/" + namelist[namelist.size() - 1].toStdString();
    }
    QStringList directorylist = QString(directory.data()).split("/");
    for(int i = 0; i < namelist.size(); ++i)
    {
        if(namelist[i] == directorylist[directorylist.size() - 1])
        {
            i++;
            filename = "";
            for(int j = i; j < namelist.size(); ++j)
            {
                filename += QString("/" + namelist[j]).toStdString();
            }
            break;
        }
    }
    filename = directory + '/' + filename;

    QOpenGLTexture * texture=new QOpenGLTexture(QImage(filename.c_str()).mirrored());
    if(texture==nullptr) qDebug()<<"texture is NULL";
    else qDebug()<<filename.c_str()<<"loaded";

    return texture->textureId();
}
QString Model::findFile(QString filepath, QString filename)
{
    QDir dir(filepath); // 创建dir目录对象
    dir.setFilter(QDir::Files | QDir::NoSymLinks); // 实现对文件的过滤
    dir.setSorting(QDir::Size | QDir::Reversed); // 实现对文件输出的排序
    QFileInfoList list = dir.entryInfoList(); // 存放文件信息列表

    for (int i = 0; i < list.size(); ++i) // 循环读取文件信息列表的信息
    {
        QFileInfo fileinfo = list.at(i); // 获取单个文件信息
        QString suffix = fileinfo.fileName(); // 获取单个文件名
        if (QString::compare(suffix, filename, Qt::CaseInsensitive) == 0)
        {
            return fileinfo.absoluteFilePath(); // 保存单个VAR文件的路径
        }
    }
    dir.setFilter(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot); // 实现对文件的过滤
    dir.setSorting(QDir::Size | QDir::Reversed); // 实现对文件输出的排序
    list = dir.entryInfoList();
    QString ret;
    for (int i = 0; i < list.size(); ++i) // 循环读取文件信息列表的信息
    {
        QFileInfo fileinfo = list.at(i); // 获取单个文件信息
        QString suffix = fileinfo.filePath(); // 获取单个文件名
        ret = findFile(suffix, filename);
        if(!ret.isEmpty())
            return ret;
    }
    return ret;
}
