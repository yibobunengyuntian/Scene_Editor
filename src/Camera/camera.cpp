
#include "camera.h"

Camera::Camera()
    : yaw(0)
    , pitch(0)
    , sensitivity(0.005f)
    , cameraPos(-5.0f,5.0f,0.0f)
    , cameraDirection(cos(yaw)*cos(pitch), sin(pitch), sin(yaw)*cos(pitch))
    , cameraRight(QVector3D::crossProduct({0.0f,1.0f,0.0f},cameraDirection))
    , cameraUp(QVector3D::crossProduct(cameraDirection,cameraRight))
    , moveSpeed(5)
    , timeId(0)
{
}

float Camera::getMoveSpeed() const
{
    return moveSpeed;
}

void Camera::setMoveSpeed(float value)
{
    moveSpeed = value;
}

float Camera::getSensitivity() const
{
    return sensitivity;
}

void Camera::setSensitivity(float value)
{
    sensitivity = value;
}

float Camera::getYaw() const
{
    return yaw;
}

void Camera::setYaw(float value)
{
    yaw = value;
}

float Camera::getPitch() const
{
    return pitch;
}

void Camera::setPitch(float value)
{
    pitch = value;
}

QVector3D Camera::getCameraPos() const
{
    return cameraPos;
}

void Camera::setCameraPos(const QVector3D &value)
{
    cameraPos = value;
    view.setToIdentity();
    view.lookAt(cameraPos,cameraPos+cameraDirection,cameraUp);
}

void Camera::init()
{
    view.setToIdentity();
    view.lookAt(cameraPos,cameraPos+cameraDirection,cameraUp);
}

QMatrix4x4 Camera::getView() const
{
    return view;
}

void Camera::moveCamera(CAMERA_MOVE_DIR dir)
{

    cameraRight = (QVector3D::crossProduct(cameraDirection,{0.0f,1.0f,0.0f}));
    cameraUp = (QVector3D::crossProduct(cameraRight,cameraDirection));
    switch (dir)
    {
    case CAMERA_MOVE_DIR::FRONT:
    {
        cameraPos += cameraDirection * moveSpeed;
    }
        break;
    case CAMERA_MOVE_DIR::BACK:
    {
        cameraPos -= cameraDirection * moveSpeed;
    }
        break;
    case CAMERA_MOVE_DIR::LEFT:
    {
        cameraPos -= cameraRight * moveSpeed;
    }
        break;
    case CAMERA_MOVE_DIR::RIGHT:
    {
        cameraPos += cameraRight * moveSpeed;
    }
        break;
    case CAMERA_MOVE_DIR::UP:
    {
        cameraPos += cameraUp * moveSpeed;
    }
    break;
    case CAMERA_MOVE_DIR::DOWN:
    {
        cameraPos -= cameraUp * moveSpeed;
    }
    break;
    default:
        break;
    }
    view.setToIdentity();
    view.lookAt(cameraPos,cameraPos+cameraDirection,cameraUp);
}

void Camera::moveCamera(QVector3D pos)
{
    cameraRight = (QVector3D::crossProduct(cameraDirection,{0.0f,1.0f,0.0f}));
    cameraUp = (QVector3D::crossProduct(cameraRight,cameraDirection));
    cameraPos = pos;
    view.setToIdentity();
    view.lookAt(cameraPos,cameraPos+cameraDirection,cameraUp);
}

void Camera::setDir(QVector3D dir)
{
    cameraDirection = dir;
    cameraRight = (QVector3D::crossProduct(cameraDirection,{0.0f,1.0f,0.0f}));
    cameraUp = (QVector3D::crossProduct(cameraRight,cameraDirection));
    view.setToIdentity();
    view.lookAt(cameraPos,cameraPos+cameraDirection,cameraUp);
}
QVector3D Camera::getDir()
{
    return cameraDirection;
}

QVector3D Camera::getUp()
{
    return cameraUp;
}

void Camera::setDir(float xoffset, float yoffset)
{
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    yaw   += xoffset;
    pitch += yoffset;
    if(pitch > 1.55f)         //将俯视角限制到[-89°,89°]，89°约等于1.55
        pitch =  1.55f;
    if(pitch < -1.55f)
        pitch = -1.55f;
    cameraDirection.setX(cos(yaw)*cos(pitch));
    cameraDirection.setY(sin(pitch));
    cameraDirection.setZ(sin(yaw)*cos(pitch));
    cameraRight = (QVector3D::crossProduct(cameraDirection,{0.0f,1.0f,0.0f}));
    cameraUp = (QVector3D::crossProduct(cameraRight,cameraDirection));
    view.setToIdentity();
    view.lookAt(cameraPos,cameraPos+cameraDirection,cameraUp);
}

void Camera::res()
{
    yaw = 0;
    pitch = 0;
    sensitivity = 0.005f;
    cameraPos = QVector3D(-5.0f,5.0f,0.0f);
    cameraDirection = QVector3D(cos(yaw)*cos(pitch), sin(pitch), sin(yaw)*cos(pitch));
    cameraRight = QVector3D::crossProduct({0.0f,1.0f,0.0f},cameraDirection);
    cameraUp = QVector3D::crossProduct(cameraDirection,cameraRight);
    moveSpeed = 5;
    view = QMatrix4x4();
    view.setToIdentity();
    view.lookAt(cameraPos,cameraPos+cameraDirection,cameraUp);
}
