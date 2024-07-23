#ifndef CAMERA_H
#define CAMERA_H

#include <QSet>
#include <QVector3D>
#include <QEvent>
#include <QWidget>
#include <QtMath>
#include <QMatrix4x4>
#include <QKeyEvent>
#include <QTime>

#define CAM_NEAR 0.1f      //相机近裁剪值
#define CAM_FAR  1000000.0f  //相机远裁剪值
enum CAMERA_MOVE_DIR{
    NONE             = -1,
    FRONT            =  0,
    BACK             =  1,
    LEFT             =  2,
    RIGHT            =  3,
    UP               =  4,
    DOWN             =  5
};


class Camera
{
public:
    Camera();

    float getMoveSpeed() const;
    void setMoveSpeed(float value);

    float getSensitivity() const;
    void setSensitivity(float value);

    float getYaw() const;
    void setYaw(float value);

    float getPitch() const;
    void setPitch(float value);

    QVector3D getCameraPos() const;
    void setCameraPos(const QVector3D &value);

    void init();                    //初始化摄像机

    QMatrix4x4 getView() const;     //获取观察矩阵

    void moveCamera(CAMERA_MOVE_DIR dir);
    void moveCamera(QVector3D pos);

    void setDir(QVector3D dir);
    QVector3D getDir();
    QVector3D getUp();

    void setDir(float xoffset, float yoffset);

    void setProjection(QMatrix4x4 mat){m_projection = mat;}
    QMatrix4x4 getProjection(){return m_projection;}

public slots:
    void res();

private:

    float yaw;                  //偏航角
    float pitch;                //俯视角
    float sensitivity;          //鼠标灵敏度

    QVector3D cameraPos;        //摄像机初始位置

    QVector3D cameraDirection;  //摄像机方向
    QVector3D cameraRight;      //摄像机右向量
    QVector3D cameraUp;         //摄像机上向量

    float moveSpeed;    //控制移动速度
    QSet<int> keys;     //记录当前被按下按键的集合

    int timeId;         //定时器id：此定时器用于完成键盘移动事件
    float deltaTime;    // 当前帧与上一帧的时间差
    float lastFrame;    // 上一帧的时间

    QMatrix4x4 view;    //观察矩阵
    QMatrix4x4 m_projection;

};

#endif // CAMERA_H
