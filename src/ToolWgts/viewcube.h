#ifndef VIEWCUBE_H
#define VIEWCUBE_H

#include <QWidget>
#include "ui_viewcube.h"
#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_3_Compatibility>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QTimer>
#include <QTime>
#include <QtMath>

class ViewCube : public QOpenGLWidget, public QOpenGLFunctions_4_3_Compatibility, public Ui_ViewCube
{
    Q_OBJECT

public:
    explicit ViewCube(QWidget *parent = nullptr);

    void setModelMat(QVector3D dir, QVector3D up);

private:
    virtual void initializeGL() override;
    virtual void resizeGL(int w,int h) override;
    virtual void paintGL() override;

//    virtual void wheelEvent(QWheelEvent *pEvent) override{};
//    virtual void mousePressEvent(QMouseEvent *pEvent) override{};
//    virtual void mouseMoveEvent(QMouseEvent *pEvent) override{};
//    virtual void mouseReleaseEvent(QMouseEvent *pEvent) override{};
//    virtual void mouseDoubleClickEvent(QMouseEvent *pEvent) override{};

//    virtual void keyPressEvent(QKeyEvent *event) override{};
//    virtual void keyReleaseEvent(QKeyEvent *event) override{};

private:
    QOpenGLShaderProgram *m_pShaderProgram = nullptr;
    QOpenGLBuffer m_VBO;
    QOpenGLVertexArrayObject m_VAO;
    QOpenGLBuffer m_IBO;

private:
    QVector<float> vertices;
    QOpenGLTexture texture;
    QTimer timer;
    QMatrix4x4 model;
};

#endif // VIEWCUBE_H
