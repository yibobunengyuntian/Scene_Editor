#include "manipulatorscale.h"
#include "PropertyManager/propertymanager.h"

ManipulatorScale::ManipulatorScale()
{
    m_manipulator_type = MANIPULATOR_TYPE::SCALE;
    m_indices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
                                    12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29};    // ��������

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
                  {QVector3D(0, 0, 1.4f), QVector3D(0, 0, 1), QVector2D()},


                  {QVector3D(0.6f, 0, 0), QVector3D(1, 0, 0), QVector2D()},
                  {QVector3D(0, 0.6f, 0), QVector3D(0, 1, 1), QVector2D()},

                  {QVector3D(0, 0.6f, 0), QVector3D(0, 1, 1), QVector2D()},
                  {QVector3D(0, 0, 0.6f), QVector3D(0, 0, 1), QVector2D()},

                  {QVector3D(0, 0, 0.6f), QVector3D(0, 0, 1), QVector2D()},
                  {QVector3D(0.6f, 0, 0), QVector3D(1, 0, 0), QVector2D()}};

    m_vertices = m_verticesBase;
}

bool ManipulatorScale::mouseDetection(QPoint pos, int width, int height)
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

    QVector4D posC1 = mat * QVector4D(0.6f, 0, 0, 1);
    posC1 /= posC1.w();
    QVector4D posC2 = mat * QVector4D(0, 0.6f, 0, 1);
    posC2 /= posC2.w();
    QVector4D posC3 = mat * QVector4D(0, 0, 0.6f, 1);
    posC3 /= posC3.w();

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

    QVector2D c1((posC1.x()+1) * w, (-posC1.y()+1) * h);
    QVector2D c2((posC2.x()+1) * w, (-posC2.y()+1) * h);
    QVector2D c3((posC3.x()+1) * w, (-posC3.y()+1) * h);

    m_vertices = m_verticesBase;
    //
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
    else if(isPosInTriangle_2D(QPointF(x, y), c1.toPointF(), c2.toPointF(), c3.toPointF()))
    {
        for(uint i = 0; i < m_vertices.size(); ++i)
        {
            if(i >= 24 && i <= 29)
            {
                m_vertices[i].Normal =  QVector3D(1, 1, 0);
            }
        }
        m_nCurrAxle = 3;
    }
    else
    {
        ret = false;
        m_nCurrAxle = -1;
    }
    return  ret;
}

void ManipulatorScale::transformByMouesmove(QPoint pos, int width, int height)
{
//    Q_UNUSED()
    //...
}

void ManipulatorScale::transformStart()
{
    if(m_bindNode)
    {
        m_startSca = m_bindNode->scale();
    }
}

void ManipulatorScale::transformEnd()
{
    QVector3D newSca = m_bindNode->scale();
    m_bindNode->setScale(m_startSca);
    g_PropertyManager->undoStack()->push(new UndoCommandScaNode(m_bindNode, newSca/m_startSca, "scale node"));
}
