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

#define CAM_NEAR 0.1f      //������ü�ֵ
#define CAM_FAR  1000000.0f  //���Զ�ü�ֵ
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

    void init();                    //��ʼ�������

    QMatrix4x4 getView() const;     //��ȡ�۲����

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

    float yaw;                  //ƫ����
    float pitch;                //���ӽ�
    float sensitivity;          //���������

    QVector3D cameraPos;        //�������ʼλ��

    QVector3D cameraDirection;  //���������
    QVector3D cameraRight;      //�����������
    QVector3D cameraUp;         //�����������

    float moveSpeed;    //�����ƶ��ٶ�
    QSet<int> keys;     //��¼��ǰ�����°����ļ���

    int timeId;         //��ʱ��id���˶�ʱ��������ɼ����ƶ��¼�
    float deltaTime;    // ��ǰ֡����һ֡��ʱ���
    float lastFrame;    // ��һ֡��ʱ��

    QMatrix4x4 view;    //�۲����
    QMatrix4x4 m_projection;

};

#endif // CAMERA_H
