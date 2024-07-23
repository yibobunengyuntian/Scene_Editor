#ifndef MANIPULATORBASE_H
#define MANIPULATORBASE_H

#include <QVector4D>
#include <QDebug>
#include <QVector2D>
#include "./../Camera/camera.h"
#include "./../PropertyManager/Nodes/node.h"
#include <QVector>
#include <QMatrix4x4>
#include <QVector3D>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <memory>

enum MANIPULATOR_TYPE{
    MANIPULATOR_TYPE_NONE = -1,
    TRANSLATE             =  0,
    ROTATE                =  1,
    SCALE                 =  2
};

enum MANIPULATOR_REFERENCE_SYSTEM{
    MANIPULATOR_REFERENCE_SYSTEM_NONE = -1,
    LOCAL                             =  0,
    PARENT                            =  1,
    WORLD                             =  2
};

class ManipulatorBase :public Node
{
public:
    ManipulatorBase();
    void bindModel(Node *model);
    void setMouesPressPos(QPoint pos);
    void setIsMouesPress(bool b);
    void changeManipulatorReferenceSystem(MANIPULATOR_REFERENCE_SYSTEM referenceSystem);
    QMatrix4x4 getModelMat(){ return m_modelMat; }

    void setupGlFunc(GLFUNC *glfuns);
    void Draw(QOpenGLShaderProgram &shader, bool isDepthMap = false);

    virtual bool mouseDetection(QPoint pos, int width, int height) = 0;
    virtual void transformByMouesmove(QPoint pos, int width, int height) = 0;
    virtual void transformStart() = 0;
    virtual void transformEnd() = 0;

    bool isPosInTriangle_2D(QPointF pos, QPointF tri_1, QPointF tri_2, QPointF tri_3);

protected:
    MANIPULATOR_TYPE m_manipulator_type = MANIPULATOR_TYPE::TRANSLATE;
    MANIPULATOR_REFERENCE_SYSTEM m_manipulatorReferenceSystem = MANIPULATOR_REFERENCE_SYSTEM::LOCAL;

    vector<Vertex> m_vertices;
    vector<unsigned int> m_indices;

    unsigned int VAO, VBO, EBO;

    Node *m_bindNode = nullptr;

    QPoint m_mouesPressPos;
    QPoint m_oldMouesPos;
    int m_nCurrAxle = -1;
    bool m_isMouesPress = false;
    bool m_pIsInit = false;
};

#endif // MANIPULATORBASE_H
