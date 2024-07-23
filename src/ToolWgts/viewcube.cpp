#include "viewcube.h"
#include "ui_viewcube.h"

ViewCube::ViewCube(QWidget *parent) :
    QOpenGLWidget(parent)
    , m_VBO(QOpenGLBuffer::VertexBuffer)
    , texture(QOpenGLTexture::Target2D)
{
    setupUi(this);
    //move(300, 0);
    setWindowFlags(Qt::FramelessWindowHint);//�޴���
    setAttribute(Qt::WA_TranslucentBackground);//����͸��
    //this->setClearMask(GL_DEPTH_BUFFER_BIT);
    setAttribute(Qt::WA_AlwaysStackOnTop);
    vertices = {
        -0.5f, -0.5f, -0.5f,  1.0f,  0.5f,
         0.5f, -0.5f, -0.5f,  0.75f, 0.5f,
         0.5f,  0.5f, -0.5f,  0.75f, 0.75f,
         0.5f,  0.5f, -0.5f,  0.75f, 0.75f,
        -0.5f,  0.5f, -0.5f,  1.0f,  0.75f,
        -0.5f, -0.5f, -0.5f,  1.0f,  0.5f,

        -0.5f, -0.5f,  0.5f,  0.25f, 0.5f,
         0.5f, -0.5f,  0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,  0.5f,  0.75f,
         0.5f,  0.5f,  0.5f,  0.5f,  0.75f,
        -0.5f,  0.5f,  0.5f,  0.25f, 0.75f,
        -0.5f, -0.5f,  0.5f,  0.25f, 0.5f,

        -0.5f,  0.5f,  0.5f,  0.75f, 0.75f,
        -0.5f,  0.5f, -0.5f,  0.5f,  0.75f,
        -0.5f, -0.5f, -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,  0.75f, 0.5f,
        -0.5f,  0.5f,  0.5f,  0.75f, 0.75f,

         0.5f,  0.5f,  0.5f,  0.0f,  0.75f,
         0.5f,  0.5f, -0.5f,  0.25f, 0.75f,
         0.5f, -0.5f, -0.5f,  0.25f, 0.5f,
         0.5f, -0.5f, -0.5f,  0.25f, 0.5f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.5f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.75f,

        -0.5f, -0.5f, -0.5f,  0.5f,  0.25f,
         0.5f, -0.5f, -0.5f,  0.75f, 0.25f,
         0.5f, -0.5f,  0.5f,  0.75f, 0.5f,
         0.5f, -0.5f,  0.5f,  0.75f, 0.5f,
        -0.5f, -0.5f,  0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,  0.5f,  0.25f,

        -0.5f,  0.5f, -0.5f,  0.25f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.5f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.5f,  0.75f,
         0.5f,  0.5f,  0.5f,  0.5f,  0.75f,
        -0.5f,  0.5f,  0.5f,  0.25f, 0.75f,
        -0.5f,  0.5f, -0.5f,  0.25f, 1.0f
    };

    QSurfaceFormat newGLFormat = this->format();  //���������
    newGLFormat.setSamples(4);
    setFormat(newGLFormat);
}


void ViewCube::setModelMat(QVector3D dir, QVector3D up)
{
    model = QMatrix4x4();
    model.lookAt(QVector3D(0, 0, 0),QVector3D(0, 0, 0)+dir,up);
    update();
}

void ViewCube::initializeGL()
{
    initializeOpenGLFunctions();        //��ʼ��opengl����
    if(m_pShaderProgram == nullptr)
    {
        m_pShaderProgram = new QOpenGLShaderProgram();
    }
    if(!m_pShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex,":/res/shader/view_cube.vert")){     //��Ӳ����붥����ɫ��
        qDebug()<<"ERROR:"<<m_pShaderProgram->log();    //����������,��ӡ������Ϣ
    }
    if(!m_pShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment,":/res/shader/view_cube.frag")){   //��Ӳ�����Ƭ����ɫ��
        qDebug()<<"ERROR:"<<m_pShaderProgram->log();    //����������,��ӡ������Ϣ
    }
    if(!m_pShaderProgram->link()){                      //������ɫ��
        qDebug()<<"ERROR:"<<m_pShaderProgram->log();    //������ӳ���,��ӡ������Ϣ
    }

    QOpenGLVertexArrayObject::Binder{&m_VAO};

    m_VBO.create();       //����VBO����
    m_VBO.bind();         //��VBO�󶨵���ǰ�Ķ��㻺�����QOpenGLBuffer::VertexBuffer����

    //���������ݷ��䵽VBO�У���һ������Ϊ����ָ�룬�ڶ�������Ϊ���ݵ��ֽڳ���
    m_VBO.allocate(vertices.data(),int(sizeof(float))*vertices.size());

    texture.create();
    texture.setData(QImage(":/res/icon/view_cube.png").mirrored());
    texture.setMinMagFilters(QOpenGLTexture::LinearMipMapLinear,QOpenGLTexture::Linear);
    texture.setWrapMode(QOpenGLTexture::DirectionS,QOpenGLTexture::Repeat);
    texture.setWrapMode(QOpenGLTexture::DirectionT,QOpenGLTexture::Repeat);


    //���ö��������ʽ�������ö���
    m_pShaderProgram->setAttributeBuffer("aPos", GL_FLOAT, 0, 3, sizeof(GLfloat) * 5);
    m_pShaderProgram->enableAttributeArray("aPos");
    m_pShaderProgram->setAttributeBuffer("aTexCoord", GL_FLOAT,sizeof(GLfloat) * 3, 2, sizeof(GLfloat) * 5);
    m_pShaderProgram->enableAttributeArray("aTexCoord");

    glEnable(GL_DEPTH_TEST);

//    glEnable (GL_POINT_SMOOTH);
//    glHint (GL_POINT_SMOOTH, GL_NICEST);
//    glEnable(GL_POINT_SMOOTH);
//    glEnable(GL_LINE_SMOOTH);
//    glEnable(GL_POLYGON_SMOOTH);
//    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
//    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
//    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_MULTISAMPLE);
}

void ViewCube::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);                //�����ӿ�����
}

void ViewCube::paintGL()
{
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);       //�����ɫ����

    m_pShaderProgram->bind();                     //ʹ��shaderProgram��ɫ����
    {
        //float time=QTime::currentTime().msecsSinceStartOfDay()/1000.0;

        //model.rotate(180*time,QVector3D(1.0f,0.5f,0.3f));
        m_pShaderProgram->setUniformValue("model",model);

        QMatrix4x4 view;
        view.translate(0.0f,0.0f,-3.0f);
        m_pShaderProgram->setUniformValue("view",view);

        QMatrix4x4 projection;
        projection.perspective(45.0f,width()/float(height()),0.1f,100.0f);
        m_pShaderProgram->setUniformValue("projection",projection);

        texture.bind(0);                                    //��texture�󶨵�����Ԫ0
        m_pShaderProgram->setUniformValue("ourTexture",0);      //��ourTexture������Ԫ0�л�ȡ��������

        QOpenGLVertexArrayObject::Binder{&m_VAO};
        glDrawArrays(GL_TRIANGLES, 0, 36);     //ʹ����0��ʼ������Ϊ36�Ķ�������������������
    }
    m_pShaderProgram->release();
}
