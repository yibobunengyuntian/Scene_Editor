#include "mesh.h"

Mesh::Mesh(MeshData *meshData, vector<Texture> textures, Node *parent)
    :Node(parent)
{
    m_nodeType = NODE_TYPE::NODE_TYPE_MESH;
    m_pMeshData = meshData;
    m_textures = textures;
}

void Mesh::setupGlFunc(GLFUNC *glfuns)
{
    m_glFuns = glfuns;
    if(!m_pMeshData)
    {
        return;
    }
    if(m_pMeshData->m_isSetup)
    {
        return;
    }
    m_pMeshData->m_isSetup = true;
    if(!m_isAddAABBIdx)
    {
        m_isAddAABBIdx = true;
        uint len = (uint)m_pMeshData->m_vertices.size();
        vector<unsigned int> aabbInd = {0+len, 1+len, 0+len, 2+len,
                                        0+len, 3+len, 4+len, 1+len,
                                        4+len, 2+len, 4+len, 5+len,
                                        6+len, 1+len, 6+len, 3+len,
                                        6+len, 5+len, 7+len, 2+len,
                                        7+len, 3+len, 7+len, 5+len};
        m_pMeshData->m_indices.insert(m_pMeshData->m_indices.end(),aabbInd.begin(),aabbInd.end());

        vector<Vertex> aabbVer = {{QVector3D(m_aabb.minPos.x(), m_aabb.minPos.y(), m_aabb.minPos.z()), QVector3D(), QVector2D()},
                                  {QVector3D(m_aabb.maxPos.x(), m_aabb.minPos.y(), m_aabb.minPos.z()), QVector3D(), QVector2D()},
                                  {QVector3D(m_aabb.minPos.x(), m_aabb.maxPos.y(), m_aabb.minPos.z()), QVector3D(), QVector2D()},
                                  {QVector3D(m_aabb.minPos.x(), m_aabb.minPos.y(), m_aabb.maxPos.z()), QVector3D(), QVector2D()},
                                  {QVector3D(m_aabb.maxPos.x(), m_aabb.maxPos.y(), m_aabb.minPos.z()), QVector3D(), QVector2D()},
                                  {QVector3D(m_aabb.maxPos.x(), m_aabb.maxPos.y(), m_aabb.maxPos.z()), QVector3D(), QVector2D()},
                                  {QVector3D(m_aabb.maxPos.x(), m_aabb.minPos.y(), m_aabb.maxPos.z()), QVector3D(), QVector2D()},
                                  {QVector3D(m_aabb.minPos.x(), m_aabb.maxPos.y(), m_aabb.maxPos.z()), QVector3D(), QVector2D()}};
        m_pMeshData->m_vertices.insert(m_pMeshData->m_vertices.end(),aabbVer.begin(),aabbVer.end());
    }



    m_glFuns->glGenVertexArrays(1, &m_pMeshData->VAO);
    m_glFuns->glGenBuffers(1, &m_pMeshData->VBO);
    m_glFuns->glGenBuffers(1,&m_pMeshData->EBO);

    m_glFuns->glBindVertexArray(m_pMeshData->VAO);
    m_glFuns->glBindBuffer(GL_ARRAY_BUFFER, m_pMeshData->VBO);

    m_glFuns->glBufferData(GL_ARRAY_BUFFER, m_pMeshData->m_vertices.size()*sizeof(Vertex),
                           &m_pMeshData->m_vertices[0], GL_STATIC_DRAW);
    m_glFuns->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pMeshData->EBO);
    m_glFuns->glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                           m_pMeshData->m_indices.size() * sizeof(unsigned int),&m_pMeshData->m_indices[0], GL_STATIC_DRAW);

    m_glFuns->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    m_glFuns->glEnableVertexAttribArray(0);
    m_glFuns->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                    (void*)offsetof(Vertex, Normal));
    m_glFuns->glEnableVertexAttribArray(1);
    m_glFuns->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                    (void*)offsetof(Vertex, TexCoords));
    m_glFuns->glEnableVertexAttribArray(2);

//    m_glFuns->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    m_glFuns->glBindBuffer(GL_ARRAY_BUFFER, 0);
    m_glFuns->glBindVertexArray(0);
}

void Mesh::setUpTextureId()
{
    for(uint i = 0; i < m_textures.size(); ++i)
    {
        m_textures[i].id = g_PropertyManager->getTextureId(QString::fromStdString(m_textures[i].path));
    }
    m_isUseColor = m_textures.size() == 0;
    foreach(auto child, m_childs)
    {
        child->setUpTextureId();
    }
}

bool Mesh::isInViewPort()
{
    QVector3D camDir = g_PropertyManager->camera()->getDir();
    QVector3D dir1 = modelMat() * m_aabb.maxPos - g_PropertyManager->camera()->getCameraPos();
    QVector3D dir2 = modelMat() * m_aabb.minPos - g_PropertyManager->camera()->getCameraPos();
    QVector3D dir3 = modelMat() * QVector3D(m_aabb.minPos.x(), m_aabb.minPos.y(),m_aabb.maxPos.z())
            - g_PropertyManager->camera()->getCameraPos();
    QVector3D dir4 = modelMat() * QVector3D(m_aabb.minPos.x(), m_aabb.maxPos.y(),m_aabb.minPos.z())
            - g_PropertyManager->camera()->getCameraPos();
    QVector3D dir5 = modelMat() * QVector3D(m_aabb.maxPos.x(), m_aabb.minPos.y(),m_aabb.minPos.z())
            - g_PropertyManager->camera()->getCameraPos();
    QVector3D dir6 = modelMat() * QVector3D(m_aabb.maxPos.x(), m_aabb.maxPos.y(),m_aabb.minPos.z())
            - g_PropertyManager->camera()->getCameraPos();
    QVector3D dir7 = modelMat() * QVector3D(m_aabb.maxPos.x(), m_aabb.minPos.y(),m_aabb.maxPos.z())
            - g_PropertyManager->camera()->getCameraPos();
    QVector3D dir8 = modelMat() * QVector3D(m_aabb.minPos.x(), m_aabb.maxPos.y(),m_aabb.maxPos.z())
            - g_PropertyManager->camera()->getCameraPos();
    if(QVector3D::dotProduct(dir1, camDir)/(dir1.length()*camDir.length()) < 0
            && QVector3D::dotProduct(dir2, camDir)/(dir2.length()*camDir.length()) < 0
            && QVector3D::dotProduct(dir3, camDir)/(dir3.length()*camDir.length()) < 0
            && QVector3D::dotProduct(dir4, camDir)/(dir4.length()*camDir.length()) < 0
            && QVector3D::dotProduct(dir5, camDir)/(dir5.length()*camDir.length()) < 0
            && QVector3D::dotProduct(dir6, camDir)/(dir6.length()*camDir.length()) < 0
            && QVector3D::dotProduct(dir7, camDir)/(dir7.length()*camDir.length()) < 0
            && QVector3D::dotProduct(dir8, camDir)/(dir8.length()*camDir.length()) < 0)
    {
        return false;
    }
    return true;
}

void Mesh::Draw(QOpenGLShaderProgram &shader, bool isDepthMap)
{
    if(m_isHide || m_glFuns == nullptr || m_pMeshData == nullptr || !isInViewPort())
    {
        return;
    }
    m_glFuns->glBindVertexArray(m_pMeshData->VAO);
    shader.setUniformValue("model", modelMat());

    if(isDepthMap)
    {
        m_glFuns->glDrawElements(GL_TRIANGLES,(GLsizei)m_pMeshData->m_indices.size() - 24,GL_UNSIGNED_INT,0);
    }
    else
    {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        if(m_isUseColor)
        {
            shader.setUniformValue("isUseColor", true);
            shader.setUniformValue("color",m_color);
        }
        else
        {
            shader.setUniformValue("isUseColor", false);
            for(unsigned int i = 0; i < m_textures.size(); i++) {
                m_glFuns->glActiveTexture(GL_TEXTURE2 + i);
                string number;
                string name = m_textures[i].type;
                if(name == "texture_diffuse")
                    number = std::to_string(diffuseNr++);
                else if(name == "texture_specular")
                    number = std::to_string(specularNr++);

                shader.setUniformValue(("material." + name + number).c_str(), 2+i);
                m_glFuns->glBindTexture(GL_TEXTURE_2D, m_textures[i].id);

            }
        }
        shader.setUniformValue("material.ambient", m_material.ambient);
        shader.setUniformValue("material.diffuse", m_material.diffuse);
        shader.setUniformValue("material.specular", m_material.specular);
        shader.setUniformValue("material.shininess", m_material.shininess);

        if(!m_isHideMesh)
        {
            shader.setUniformValue("isMesh", true);
            m_glFuns->glLineWidth(2);
            //        m_glFuns->glPolygonOffset(-1.0f, -1.0f);      // Shift depth values
            //        m_glFuns->glEnable(GL_POLYGON_OFFSET_LINE);
            // m_glFuns->glEnable(GL_POLYGON_OFFSET_FILL);//开启多边形偏移
            //        m_glFuns->glPolygonOffset(1.0f, 1.0f); // 调整偏移量的值，根据需要进行调整
            //        m_glFuns->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//将绘制模式改为线
            m_glFuns->glDrawElements(GL_LINES,(GLsizei)m_pMeshData->m_indices.size() - 24,GL_UNSIGNED_INT,0);
            //        m_glFuns->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//将绘制模式改为面
            m_glFuns->glLineWidth(1);
            shader.setUniformValue("isMesh", false);
        }
        if(!m_isHideAABB)
        {
            shader.setUniformValue("isMesh", true);
            m_glFuns->glLineWidth(2);
            m_glFuns->glDrawElements(GL_LINES,24,GL_UNSIGNED_INT,(const GLvoid*)((m_pMeshData->m_indices.size() - 24)*sizeof(uint)));
            m_glFuns->glLineWidth(1);
            shader.setUniformValue("isMesh", false);
        }

        if(m_isHideOutLine)
        {
            m_glFuns->glDrawElements(GL_TRIANGLES,(GLsizei)m_pMeshData->m_indices.size() - 24,GL_UNSIGNED_INT,0);
        }
        else
        {
            // 允许向模板缓冲区写入值
            m_glFuns->glStencilMask(0xFF);
            // 告诉OpenGL应该对模板缓冲中所有片段进行绘制，同时在通过模板测试及深度测试后模板缓冲区的内容会被替换成1(被替换的操作是由glStencilOp指定的，替换的值则是由glStencilFunc给定)
            m_glFuns->glStencilFunc(GL_ALWAYS, 1, 0xFF);
            m_glFuns->glDrawElements(GL_TRIANGLES,(GLsizei)m_pMeshData->m_indices.size() - 24,GL_UNSIGNED_INT,0);
            shader.setUniformValue("isMesh", true);

            //绘制模型边框
            // 绘制不等于1的区域，由于第二次会模型会被放大，所以被放大的那一部分会被绘制
            m_glFuns->glStencilMask(0x00);//关闭写进模板缓冲，现在模板缓冲里只有两个箱子的缓冲值是“1”
            //        m_glFuns->glDisable(GL_DEPTH_TEST);
            m_glFuns->glStencilFunc(GL_NOTEQUAL, 1, 0xFF);

            //使轮廓尽可能真实
            QMatrix4x4 mat = modelMat();
            double xl = (m_aabb.maxPos.x() - m_aabb.minPos.x()) * m_scale.x();
            double yl = (m_aabb.maxPos.y() - m_aabb.minPos.y()) * m_scale.y();
            double zl = (m_aabb.maxPos.z() - m_aabb.minPos.z()) * m_scale.z();
            m_centrePos = (m_aabb.maxPos + m_aabb.minPos)/2;
            QVector3D cont = 0.1f *(g_PropertyManager->camera()->getCameraPos() - mat * m_centrePos)/10;
            double cx = QVector2D(cont.y(), cont.z()).length()+0.001;
            double cy = QVector2D(cont.x(), cont.z()).length()+0.001;
            double cz = QVector2D(cont.y(), cont.x()).length()+0.001;
            mat.scale(QVector3D(((xl + cx)/xl),
                                ((yl + cy)/yl),
                                ((zl + cz)/zl)));
            shader.setUniformValue("model", mat);

            m_glFuns->glDrawElements(GL_TRIANGLES,(GLsizei)m_pMeshData->m_indices.size() - 24,GL_UNSIGNED_INT,0);
            mat = modelMat();
            mat.scale(QVector3D(((xl - cx)/xl),
                                ((yl - cy)/yl),
                                ((zl - cz)/zl)));
            shader.setUniformValue("model", mat);

            m_glFuns->glDrawElements(GL_TRIANGLES,(GLsizei)m_pMeshData->m_indices.size() - 24,GL_UNSIGNED_INT,0);
            //        m_glFuns->glEnable(GL_DEPTH_TEST);

            shader.setUniformValue("isMesh", false);
            m_glFuns->glStencilMask(0xFF);//重新开启写进模板缓冲
            m_glFuns->glStencilFunc(GL_ALWAYS, 1, 0xFF); //因为glClear()函数并不能清除掉模板缓冲，所以我们需要还原缓冲模板缓冲功能函数glStencilFunc
        }
        foreach(auto node, m_childs)
        {
            node->Draw(shader);
        }
    }
    m_glFuns->glBindVertexArray(0);
}
