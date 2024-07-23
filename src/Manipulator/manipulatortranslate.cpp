#include "manipulatortranslate.h"
#include "PropertyManager/propertymanager.h"

ManipulatorTranslate::ManipulatorTranslate()
{
    m_manipulator_type = MANIPULATOR_TYPE::TRANSLATE;
    m_indices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
                                    12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};    // 顶点索引

    m_verticesBase = {{QVector3D(0, 0, 0), QVector3D(1, 0, 0), QVector2D()},
                  {QVector3D(1, 0, 0), QVector3D(1, 0, 0), QVector2D()},

                  {QVector3D(0, 0, 0), QVector3D(0, 1, 1), QVector2D()},
                  {QVector3D(0, 1, 0), QVector3D(0, 1, 1), QVector2D()},

                  {QVector3D(0, 0, 0), QVector3D(0, 0, 1), QVector2D()},
                  {QVector3D(0, 0, 1), QVector3D(0, 0, 1), QVector2D()},


                  {QVector3D(1, 0.2f, 0), QVector3D(1, 0, 0), QVector2D()},
                  {QVector3D(1, -0.2f, 0), QVector3D(1, 0, 0), QVector2D()},

                  {QVector3D(1, 0.2f, 0), QVector3D(1, 0, 0), QVector2D()},
                  {QVector3D(1.4f, 0, 0), QVector3D(1, 0, 0), QVector2D()},

                  {QVector3D(1, -0.2f, 0), QVector3D(1, 0, 0), QVector2D()},
                  {QVector3D(1.4f, 0, 0), QVector3D(1, 0, 0), QVector2D()},


                  {QVector3D(-0.2f, 1, 0), QVector3D(0, 1, 1), QVector2D()},
                  {QVector3D(0, 1.4f, 0), QVector3D(0, 1, 1), QVector2D()},

                  {QVector3D(-0.2f, 1, 0), QVector3D(0, 1, 1), QVector2D()},
                  {QVector3D(0.2f, 1, 0), QVector3D(0, 1, 1), QVector2D()},

                  {QVector3D(0, 1.4f, 0), QVector3D(0, 1, 1), QVector2D()},
                  {QVector3D(0.2f, 1, 0), QVector3D(0, 1, 1), QVector2D()},


                  {QVector3D(-0.2f, 0, 1), QVector3D(0, 0, 1), QVector2D()},
                  {QVector3D(0.2f, 0, 1), QVector3D(0, 0, 1), QVector2D()},

                  {QVector3D(-0.2f, 0, 1), QVector3D(0, 0, 1), QVector2D()},
                  {QVector3D(0, 0, 1.4f), QVector3D(0, 0, 1), QVector2D()},

                  {QVector3D(0.2f, 0, 1), QVector3D(0, 0, 1), QVector2D()},
                  {QVector3D(0, 0, 1.4f), QVector3D(0, 0, 1), QVector2D()}};

    m_vertices = m_verticesBase;
}

bool ManipulatorTranslate::mouseDetection(QPoint pos, int width, int height)
{
    if(m_isMouesPress)
    {
        return true;
    }
    bool ret = true;
    QMatrix4x4 mat = g_PropertyManager->camera()->getProjection() * g_PropertyManager->camera()->getView() * m_modelMat;
    QVector4D posX1 = mat * QVector4D(1, 0.2f, 0, 1);
    posX1 /= posX1.w();
    QVector4D posX2 = mat * QVector4D(1, -0.2f, 0, 1);
    posX2 /= posX2.w();
    QVector4D posX3 = mat * QVector4D(1.4f, 0, 0, 1);
    posX3 /= posX3.w();

    QVector4D posY1 = mat * QVector4D(-0.2f, 1, 0, 1);
    posY1 /= posY1.w();
    QVector4D posY2 = mat * QVector4D(0, 1.4f, 0, 1);
    posY2 /= posY2.w();
    QVector4D posY3 = mat * QVector4D(0.2f, 1, 0, 1);
    posY3 /= posY3.w();

    QVector4D posZ1 = mat * QVector4D(-0.2f, 0, 1, 1);
    posZ1 /= posZ1.w();
    QVector4D posZ2 = mat * QVector4D(0.2f, 0, 1, 1);
    posZ2 /= posZ2.w();
    QVector4D posZ3 = mat * QVector4D(0, 0, 1.4f, 1);
    posZ3 /= posZ3.w();

    int x = pos.x();
    int y = pos.y();
    int w = width/2;
    int h = height/2;
    QVector2D x1((posX1.x()+1) * w, (-posX1.y()+1) * h);
    QVector2D x2((posX2.x()+1) * w, (-posX2.y()+1) * h);
    QVector2D x3((posX3.x()+1) * w, (-posX3.y()+1) * h);

    QVector2D y1((posY1.x()+1) * w, (-posY1.y()+1) * h);
    QVector2D y2((posY2.x()+1) * w, (-posY2.y()+1) * h);
    QVector2D y3((posY3.x()+1) * w, (-posY3.y()+1) * h);

    QVector2D z1((posZ1.x()+1) * w, (-posZ1.y()+1) * h);
    QVector2D z2((posZ2.x()+1) * w, (-posZ2.y()+1) * h);
    QVector2D z3((posZ3.x()+1) * w, (-posZ3.y()+1) * h);
    m_vertices = m_verticesBase;
    //判断是否在三角形内
    if(isPosInTriangle_2D(QPointF(x, y), x1.toPointF(), x2.toPointF(), x3.toPointF()))
    {
        for(uint i = 0; i < m_vertices.size(); ++i)
        {
            if((i <= 1) || (i >= 6 && i <= 11))
            {
                m_vertices[i].Normal =  QVector3D(1, 1, 0);
            }
        }
        m_nCurrAxle = 0;
    }
    else if(isPosInTriangle_2D(QPointF(x, y), y1.toPointF(), y2.toPointF(), y3.toPointF()))
    {
        for(uint i = 0; i < m_vertices.size(); ++i)
        {
            if((i >= 2 && i <= 3) || (i >= 12 && i <= 17))
            {
                m_vertices[i].Normal =  QVector3D(1, 1, 0);
            }
        }
        m_nCurrAxle = 1;
    }
    else if(isPosInTriangle_2D(QPointF(x, y), z1.toPointF(), z2.toPointF(), z3.toPointF()))
    {
        for(uint i = 0; i < m_vertices.size(); ++i)
        {
            if((i >= 4 && i <= 5) || (i >= 18 && i <= 23))
            {
                m_vertices[i].Normal =  QVector3D(1, 1, 0);
            }
        }
        m_nCurrAxle = 2;
    }
    else
    {
        ret = false;
        m_nCurrAxle = -1;
    }
    return  ret;
}

void ManipulatorTranslate::transformByMouesmove(QPoint pos, int width, int height)
{
    QPoint tmp = pos - m_oldMouesPos;
    m_oldMouesPos = pos;
    double f = 1;
    QMatrix4x4 tmpMat = QMatrix4x4();
    Node *pNode = m_bindNode->parent();
    // if(!pNode)
    {
        tmpMat.rotate(-m_bindNode->rotate().x(), QVector3D(1, 0, 0));
        tmpMat.rotate(-m_bindNode->rotate().z(), QVector3D(0, 0, 1));
        tmpMat.rotate(-m_bindNode->rotate().y(), QVector3D(0, 1, 0));
    }
    // else
    // {
    //     tmpMat.rotate(-(m_bindNode->getRotate() - pNode->getRotate()).x(), QVector3D(1, 0, 0));
    //     tmpMat.rotate(-(m_bindNode->getRotate() - pNode->getRotate()).z(), QVector3D(0, 0, 1));
    //     tmpMat.rotate(-(m_bindNode->getRotate() - pNode->getRotate()).y(), QVector3D(0, 1, 0));
    // }
    QMatrix4x4 mat = g_PropertyManager->camera()->getProjection() * g_PropertyManager->camera()->getView() * m_modelMat;
    int w = width/2;
    int h = height/2;
    float distanceCoefficient = ((g_PropertyManager->camera()->getCameraPos() - m_modelMat * m_centrePos).length())/height*578/800;
    switch (m_nCurrAxle)
    {
    case 0:
    {
        if(pNode)
        {
//            f = 1/m_bindNode->parent()->scale().x();
        }
        QVector4D vec1 = mat * QVector4D(0, 0, 0, 1);
        vec1 /= vec1.w();
        QPoint pos1(int((vec1.x()+1) * w), int((-vec1.y()+1) * h));
        QVector4D vec2 = mat * QVector4D(1, 0, 0, 1);
        vec2 /= vec2.w();
        QPoint pos2(int((vec2.x()+1) * w), int((-vec2.y()+1) * h));
        QPoint p = pos2 - pos1;
        m_bindNode->translate(QVector3D(float((tmp.x()*p.x() + tmp.y()*p.y())/sqrt(p.x() *p.x() + p.y() * p.y())
            *distanceCoefficient * f), 0, 0)*tmpMat);
    }
        break;
    case 1:
    {
        if(pNode)
        {
//            f = 1/m_bindNode->parent()->scale().y();
        }
        QVector4D vec1 = mat * QVector4D(0, 0, 0, 1);
        vec1 /= vec1.w();
        QPoint pos1(int((vec1.x()+1) * w), int((-vec1.y()+1) * h));
        QVector4D vec2 = mat * QVector4D(0, 1, 0, 1);
        vec2 /= vec2.w();
        QPoint pos2(int((vec2.x()+1) * w), int((-vec2.y()+1) * h));
        QPoint p = pos2 - pos1;
        m_bindNode->translate(QVector3D(0, float((tmp.x()*p.x() + tmp.y()*p.y())/sqrt(p.x() *p.x() + p.y() * p.y())
            *distanceCoefficient * f), 0)*tmpMat);
    }
        break;
    case 2:
    {
        if(pNode)
        {
//            f = 1/m_bindNode->parent()->scale().z();
        }
        QVector4D vec1 = mat * QVector4D(0, 0, 0, 1);
        vec1 /= vec1.w();
        QPoint pos1(int((vec1.x()+1) * w), int((-vec1.y()+1) * h));
        QVector4D vec2 = mat * QVector4D(0, 0, 1, 1);
        vec2 /= vec2.w();
        QPoint pos2(int((vec2.x()+1) * w), int((-vec2.y()+1) * h));
        QPoint p = pos2 - pos1;
        m_bindNode->translate(QVector3D(0, 0, float((tmp.x()*p.x() + tmp.y()*p.y())/sqrt(p.x() *p.x() + p.y() * p.y())
            *distanceCoefficient * f))*tmpMat);
    }
        break;
    }
    g_PropertyManager->setIsNeedSave(true);
}

void ManipulatorTranslate::transformStart()
{
    if(m_bindNode)
    {
        m_startPos = m_bindNode->translate();
    }
}

void ManipulatorTranslate::transformEnd()
{
    QVector3D newPos = m_bindNode->translate();
    m_bindNode->setTranslate(m_startPos);
    g_PropertyManager->undoStack()->push(new UndoCommandTraNode(m_bindNode, newPos - m_startPos, "translate node"));
}
