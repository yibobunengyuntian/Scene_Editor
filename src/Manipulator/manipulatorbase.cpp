#include "manipulatorbase.h"
#include "PropertyManager/propertymanager.h"

ManipulatorBase::ManipulatorBase()
{

}

void ManipulatorBase::bindModel(Node *model)
{
    m_bindNode = model;

    //...
}

void ManipulatorBase::setMouesPressPos(QPoint pos)
{
    m_mouesPressPos = pos;
    m_oldMouesPos = pos;
}

void ManipulatorBase::setIsMouesPress(bool b)
{
    if(m_isMouesPress == b)
    {
        return;
    }
    m_isMouesPress = b;
    if(b)
    {
        transformStart();
    }
    else
    {
        transformEnd();
    }
}

void ManipulatorBase::changeManipulatorReferenceSystem(MANIPULATOR_REFERENCE_SYSTEM referenceSystem)
{
    switch (referenceSystem) {
    case MANIPULATOR_REFERENCE_SYSTEM::LOCAL:
    {

    }
        break;
    case MANIPULATOR_REFERENCE_SYSTEM::WORLD:
    {

    }
        break;
    case MANIPULATOR_REFERENCE_SYSTEM::PARENT:
    {

    }
        break;
    default:
        break;
    }
}


void ManipulatorBase::setupGlFunc(GLFUNC *glfuns)
{

    if(m_pIsInit == false)
    {
        if(m_indices.size() == 0 || m_vertices.size() == 0)
        {
            return;
        }
        m_pIsInit = true;
        m_glFuns = glfuns;
        m_glFuns->glGenVertexArrays(1, &VAO);
        m_glFuns->glGenBuffers(1, &VBO);
        m_glFuns->glGenBuffers(1,&EBO);

        m_glFuns->glBindVertexArray(VAO);
        m_glFuns->glBindBuffer(GL_ARRAY_BUFFER, VBO);

        m_glFuns->glBufferData(GL_ARRAY_BUFFER, m_vertices.size()*sizeof(Vertex),
                               &m_vertices[0], GL_STATIC_DRAW);
        m_glFuns->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        m_glFuns->glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                               m_indices.size() * sizeof(unsigned int),&m_indices[0], GL_STATIC_DRAW);

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
    else
    {
        m_glFuns->glBindVertexArray(VAO);
        m_glFuns->glBindBuffer(GL_ARRAY_BUFFER, VBO);

        m_glFuns->glBufferData(GL_ARRAY_BUFFER, m_vertices.size()*sizeof(Vertex),
                               &m_vertices[0], GL_STATIC_DRAW);
    }
}

void ManipulatorBase::Draw(QOpenGLShaderProgram &shader, bool isDepthMap)
{
    Q_UNUSED(isDepthMap)
    if(m_bindNode == nullptr || m_glFuns == nullptr)
    {
        return;
    }

    m_glFuns->glDisable(GL_DEPTH_TEST);
    m_modelMat = m_bindNode->modelMat();
    m_modelMat.scale(1/m_bindNode->scale().x(), 1/m_bindNode->scale().y(), 1/m_bindNode->scale().z());
    m_modelMat.scale((g_PropertyManager->camera()->getCameraPos() - m_modelMat * m_centrePos).length()/10);
    m_glFuns->glBindVertexArray(VAO);
    shader.setUniformValue("model", m_modelMat);

    shader.setUniformValue("NormalIsColor", true);
    m_glFuns->glLineWidth(2);
    m_glFuns->glDrawElements(GL_LINES,(GLsizei)m_indices.size(),GL_UNSIGNED_INT,0);
    m_glFuns->glLineWidth(1);
    shader.setUniformValue("NormalIsColor", false);
    m_glFuns->glBindVertexArray(0);
    m_glFuns->glEnable(GL_DEPTH_TEST);
}

bool ManipulatorBase::isPosInTriangle_2D(QPointF pos, QPointF tri_1, QPointF tri_2, QPointF tri_3)
{
    return abs(tri_1.x()*(tri_2.y()-tri_3.y()) + tri_2.x()*(tri_3.y()-tri_1.y()) + tri_3.x()*(tri_1.y()-tri_2.y()))/2*1.1>=
        (abs(pos.x()*(tri_2.y()-tri_3.y()) + tri_2.x()*(tri_3.y()-pos.y()) + tri_3.x()*(pos.y()-tri_2.y()))/2 +
         abs(tri_1.x()*(pos.y()-tri_3.y()) + pos.x()*(tri_3.y()-tri_1.y()) + tri_3.x()*(tri_1.y()-pos.y()))/2 +
         abs(tri_1.x()*(tri_2.y()-pos.y()) + tri_2.x()*(pos.y()-tri_1.y()) + pos.x()*(tri_1.y()-tri_2.y()))/2);
}
