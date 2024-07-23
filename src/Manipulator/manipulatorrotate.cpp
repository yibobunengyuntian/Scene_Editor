#include "manipulatorrotate.h"
#include "PropertyManager/propertymanager.h"

ManipulatorRotate::ManipulatorRotate()
{
    m_manipulator_type = MANIPULATOR_TYPE::ROTATE;
    m_indices = {0, 1, 2, 3, 4, 5};    //

    m_vertices = {{QVector3D(0, 0, 0), QVector3D(1, 0, 0), QVector2D()},
                  {QVector3D(1, 0, 0), QVector3D(1, 0, 0), QVector2D()},

                  {QVector3D(0, 0, 0), QVector3D(0, 1, 1), QVector2D()},
                  {QVector3D(0, 1, 0), QVector3D(0, 1, 1), QVector2D()},

                  {QVector3D(0, 0, 0), QVector3D(0, 0, 1), QVector2D()},
                  {QVector3D(0, 0, 1), QVector3D(0, 0, 1), QVector2D()}};
    m_indices.push_back(m_indices[m_indices.size() - 1] + 1);
    for(int i = 0; i < 360; ++i)
    {
        float rad = i*M_PI/180;
        m_vertices.push_back({QVector3D(qCos(rad), 0, qSin(rad)), QVector3D(0, 1, 1), QVector2D()});
        m_indices.push_back(m_indices[m_indices.size() - 1] + 1);
        m_indices.push_back(m_indices[m_indices.size() - 1]);
    }
    m_indices.pop_back();
    m_indices.pop_back();
    m_indices.pop_back();
    m_indices.push_back(m_indices[m_indices.size() - 1] + 1);
    for(int i = 0; i < 360; ++i)
    {
        float rad = i*M_PI/180;
        m_vertices.push_back({QVector3D(0, qCos(rad), qSin(rad)), QVector3D(1, 0, 0), QVector2D()});
        m_indices.push_back(m_indices[m_indices.size() - 1] + 1);
        m_indices.push_back(m_indices[m_indices.size() - 1]);
    }
    m_indices.pop_back();
    m_indices.pop_back();
    m_indices.pop_back();
    m_indices.push_back(m_indices[m_indices.size() - 1] + 1);
    for(int i = 0; i < 360; ++i)
    {
        float rad = i*M_PI/180;
        m_vertices.push_back({QVector3D(qCos(rad), qSin(rad), 0), QVector3D(0, 0, 1), QVector2D()});
        m_indices.push_back(m_indices[m_indices.size() - 1] + 1);
        m_indices.push_back(m_indices[m_indices.size() - 1]);
    }
    m_indices.pop_back();
    m_indices.pop_back();
    m_indices.pop_back();
}

bool ManipulatorRotate::mouseDetection(QPoint pos, int width, int height)
{
    if(m_isMouesPress)
    {
        return true;
    }
    float len = 2;
    int x = pos.x();
    int y = pos.y();
    QVector2D pos_mou = QVector2D(x, y);
    QMatrix4x4 mat = g_PropertyManager->camera()->getProjection() * g_PropertyManager->camera()->getView() * m_modelMat;
    for(uint i = 6; i < m_vertices.size(); ++i)
    {
        QVector3D p = m_vertices[i].Position;
        QVector4D pos = mat * QVector4D(p, 1);
        pos /= pos.w();
        QVector2D pos_2d((pos.x()+1) * width/2, (-pos.y()+1) * height/2);

        if((pos_2d - pos_mou).length() < len)
        {
            len = (pos_2d - pos_mou).length();
            m_selPos = p;
            m_nSelIndex = i;
        }
        if(i >= 6 && i < 366)
        {
            m_vertices[i].Normal = QVector3D(0, 1, 1);
        }
        else if(i >= 366 && i <= 726)
        {
            m_vertices[i].Normal = QVector3D(1, 0, 0);
        }
        else
        {
            m_vertices[i].Normal = QVector3D(0, 0, 1);
        }
    }
    m_vertices[0].Normal = QVector3D(1, 0, 0);
    m_vertices[1].Normal = QVector3D(1, 0, 0);
    m_vertices[2].Normal = QVector3D(0, 1, 1);
    m_vertices[3].Normal = QVector3D(0, 1, 1);
    m_vertices[4].Normal = QVector3D(0, 0, 1);
    m_vertices[5].Normal = QVector3D(0, 0, 1);
    if(qAbs(len - 2) < 0.00001f)
    {
        m_nCurrAxle = -1;
        return false;
    }

    if(m_nSelIndex >= 6 && m_nSelIndex < 366)
    {
        m_nCurrAxle = 1;
        m_vertices[2].Normal = QVector3D(1, 1, 0);
        m_vertices[3].Normal = QVector3D(1, 1, 0);
        for(uint i = 6; i < 366; ++i)
        {
            m_vertices[i].Normal = QVector3D(1, 1 ,0);
        }
    }
    else if(m_nSelIndex >= 367 && m_nSelIndex < 726)
    {
        m_nCurrAxle = 0;
        m_vertices[0].Normal = QVector3D(1, 1, 0);
        m_vertices[1].Normal = QVector3D(1, 1, 0);
        for(uint i = 367; i < 726; ++i)
        {
            m_vertices[i].Normal = QVector3D(1, 1 ,0);
        }
    }
    else
    {
        m_nCurrAxle = 2;
        m_vertices[4].Normal = QVector3D(1, 1, 0);
        m_vertices[5].Normal = QVector3D(1, 1, 0);
        for(uint i = 726; i < m_vertices.size(); ++i)
        {
            m_vertices[i].Normal = QVector3D(1, 1 ,0);
        }
    }
    m_oldRotate = m_bindNode->rotate();
    m_oldMatri = m_modelMat;
    return  true;
}

void ManipulatorRotate::transformByMouesmove(QPoint pos, int width, int height)
{
    int x = pos.x();
    int y = pos.y();
    QVector2D pos_mou = QVector2D(x, y);
    QMatrix4x4 mat = g_PropertyManager->camera()->getProjection() * g_PropertyManager->camera()->getView() * m_oldMatri;
    float len = 5000;
    QVector3D selPos;
    uint index = 0;
    switch (m_nCurrAxle)
    {
    case 0:
    {
        for(uint i = 367; i < 726; ++i)
        {
            QVector3D p = m_vertices[i].Position;
            QVector4D pos = mat * QVector4D(p, 1);
            pos /= pos.w();
            QVector2D pos_2d((pos.x()+1) * width/2, (-pos.y()+1) * height/2);

            if((pos_2d - pos_mou).length() < len)
            {
                len = (pos_2d - pos_mou).length();
                selPos = p;
                index = i;
            }
        }
        // qDebug() << __FUNCTION__ << selPos <<m_nSelIndex << index;
        m_bindNode->setRotate(QVector3D(index - m_nSelIndex, 0, 0) + m_oldRotate);
        // m_nSelIndex = index;
    }
        break;
    case 1:
    {
        for(uint i = 6; i < 366; ++i)
        {
            QVector3D p = m_vertices[i].Position;
            QVector4D pos = mat * QVector4D(p, 1);
            pos /= pos.w();
            QVector2D pos_2d((pos.x()+1) * width/2, (-pos.y()+1) * height/2);

            if((pos_2d - pos_mou).length() < len)
            {
                len = (pos_2d - pos_mou).length();
                selPos = p;
                index = i;
            }
        }
        // qDebug() << __FUNCTION__ << selPos <<m_nSelIndex << index;
        m_bindNode->setRotate(QVector3D(0, m_nSelIndex - index, 0) + m_oldRotate);
        // m_nSelIndex = index;
    }
        break;
    case 2:
    {
        for(uint i = 726; i < m_vertices.size(); ++i)
        {
            QVector3D p = m_vertices[i].Position;
            QVector4D pos = mat * QVector4D(p, 1);
            pos /= pos.w();
            QVector2D pos_2d((pos.x()+1) * width/2, (-pos.y()+1) * height/2);

            if((pos_2d - pos_mou).length() < len)
            {
                len = (pos_2d - pos_mou).length();
                selPos = p;
                index = i;
            }
        }
        // qDebug() << __FUNCTION__ << selPos <<m_nSelIndex << index;
        m_bindNode->setRotate(QVector3D(0, 0, index - m_nSelIndex) + m_oldRotate);
        // m_nSelIndex = index;
    }
        break;
    }
    g_PropertyManager->setIsNeedSave(true);
}

void ManipulatorRotate::transformStart()
{
    if(m_bindNode)
    {
        m_startRot = m_bindNode->rotate();
    }
}

void ManipulatorRotate::transformEnd()
{
    QVector3D newRot = m_bindNode->rotate();
    m_bindNode->setRotate(m_startRot);
    g_PropertyManager->undoStack()->push(new UndoCommandRotNode(m_bindNode, newRot - m_startRot, "rotate node"));
}
