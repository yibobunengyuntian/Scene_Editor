#ifndef RENDERWGT_H
#define RENDERWGT_H

#include "plc.h"

class RenderWgt : public QOpenGLWidget, public GLFUNC
{
    Q_OBJECT

public:
    explicit RenderWgt(QWidget *parent = nullptr);

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w,int h) override;
    virtual void paintGL() override;

    virtual void wheelEvent(QWheelEvent *pEvent) override;
    virtual void mousePressEvent(QMouseEvent *pEvent) override;
    virtual void mouseMoveEvent(QMouseEvent *pEvent) override;
    virtual void mouseReleaseEvent(QMouseEvent *pEvent) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *pEvent) override;

    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;

    void dropEvent(QDropEvent *event) override;								//放下动作
    void dragEnterEvent(QDragEnterEvent *event) override;					//托到进入窗口动作
    void dragMoveEvent(QDragMoveEvent *event) override;						//拖着物体在窗口移动
    void dragLeaveEvent(QDragLeaveEvent *event) override;					//拖走了没有释放

    void initialize();

    void initializeShader();
    void createFloor();
    void createDepthTexture();

    void loadWorld();
    void loadModel();

    void renderDepthMap();
    void renderNode();

public:
    QVector4D worldPosFromViewPort(int posX, int posY);
    void setBackgroundColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0);

public slots:
    void addModelByFile();
    void setLightDir(QVector3D dir){
        m_lightDir = dir;
    }
    void setUseCameraDir(bool is){m_isUseCameraDir = is;}
    void setHiddenShadow(bool is){m_isDepthMap = is; m_isUpDateShadow = true;}
    void setShadowUpDate(int n){m_shadowUpDate = n; m_isUpDateShadow = true;}

private:
    QOpenGLShaderProgram m_ShaderProgram;
    QOpenGLShaderProgram m_DepthMapShaderProgram;

    CameraTool *m_pCameraTool = nullptr;
    ViewCube *m_pViewCube = nullptr;
    QMatrix4x4 m_projection;
private:
    QTimer timer;
//    Camera camera;

    bool m_isKeyCtrlDown = false;
    bool m_isMouseLeftDown = false;
    bool m_isMouseRightDown = false;
    bool m_isSelectManipulator = false;
    QPoint m_mouseRightDownPos;
    QCursor cursor;
    QVector3D m_oldDir;
    float yaw = 0;                  //偏航角
    float pitch = 0;                //俯视角
    float sensitivity = 0.005f;          //鼠标灵敏度

    QMap<Qt::Key, bool> m_keys;
    Node *m_pLoadModel = nullptr;
    QList<Node *> m_loadModelList;
    Node *m_pCopyNode = nullptr;
    Node *m_pTmpNode = nullptr;

    bool m_isLoadModel = false;
    QString m_modelPath;
    TipWgt *m_pTip = nullptr;
    bool m_isCopyModel = false;
    bool m_isMouseMoveCamera = false;
    bool m_isUseCameraDir = false;
    bool m_isDepthMap = false;
    bool m_isUpDateShadow = true;
    QVector3D m_lightDir = QVector3D(-0.2f, -1.0f, -0.3f);
    QVector4D m_backgroundColor = QVector4D(0, 0.4f, 0.6f, 1.0f);

    const unsigned int m_SHADOW_WIDTH = 10240, m_SHADOW_HEIGHT = 10240;
    unsigned int m_depthMapFBO;
    unsigned int m_depthMap;
    int m_shadowUpDate = 0;
    QVector3D m_tmpLightDir;
    QMutex m_loadModelMutex;
};

#endif // RENDERWGT_H
