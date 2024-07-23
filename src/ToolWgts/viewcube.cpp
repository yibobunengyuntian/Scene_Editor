#include "viewcube.h"
#include "ui_viewcube.h"

ViewCube::ViewCube(QWidget *parent) :
    QOpenGLWidget(parent)
    , m_VBO(QOpenGLBuffer::VertexBuffer)
    , texture(QOpenGLTexture::Target2D)
{
    setupUi(this);
    //move(300, 0);
    setWindowFlags(Qt::FramelessWindowHint);//无窗体
    setAttribute(Qt::WA_TranslucentBackground);//背景透明
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

    QSurfaceFormat newGLFormat = this->format();  //开启抗锯齿
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
    initializeOpenGLFunctions();        //初始化opengl函数
    if(m_pShaderProgram == nullptr)
    {
        m_pShaderProgram = new QOpenGLShaderProgram();
    }
    if(!m_pShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex,":/res/shader/view_cube.vert")){     //添加并编译顶点着色器
        qDebug()<<"ERROR:"<<m_pShaderProgram->log();    //如果编译出错,打印报错信息
    }
    if(!m_pShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment,":/res/shader/view_cube.frag")){   //添加并编译片段着色器
        qDebug()<<"ERROR:"<<m_pShaderProgram->log();    //如果编译出错,打印报错信息
    }
    if(!m_pShaderProgram->link()){                      //链接着色器
        qDebug()<<"ERROR:"<<m_pShaderProgram->log();    //如果链接出错,打印报错信息
    }

    QOpenGLVertexArrayObject::Binder{&m_VAO};

    m_VBO.create();       //生成VBO对象
    m_VBO.bind();         //将VBO绑定到当前的顶点缓冲对象（QOpenGLBuffer::VertexBuffer）中

    //将顶点数据分配到VBO中，第一个参数为数据指针，第二个参数为数据的字节长度
    m_VBO.allocate(vertices.data(),int(sizeof(float))*vertices.size());

    texture.create();
    texture.setData(QImage(":/res/icon/view_cube.png").mirrored());
    texture.setMinMagFilters(QOpenGLTexture::LinearMipMapLinear,QOpenGLTexture::Linear);
    texture.setWrapMode(QOpenGLTexture::DirectionS,QOpenGLTexture::Repeat);
    texture.setWrapMode(QOpenGLTexture::DirectionT,QOpenGLTexture::Repeat);


    //设置顶点解析格式，并启用顶点
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
    glViewport(0,0,w,h);                //定义视口区域
}

void ViewCube::paintGL()
{
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);       //清除颜色缓存

    m_pShaderProgram->bind();                     //使用shaderProgram着色程序
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

        texture.bind(0);                                    //将texture绑定到纹理单元0
        m_pShaderProgram->setUniformValue("ourTexture",0);      //让ourTexture从纹理单元0中获取纹理数据

        QOpenGLVertexArrayObject::Binder{&m_VAO};
        glDrawArrays(GL_TRIANGLES, 0, 36);     //使用以0开始，长度为36的顶点数据来绘制三角形
    }
    m_pShaderProgram->release();
}
