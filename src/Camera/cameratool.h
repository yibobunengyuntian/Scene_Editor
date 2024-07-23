#ifndef CAMERATOOL_H
#define CAMERATOOL_H

#include <QWidget>
#include "ui_cameratool.h"
#include <QVector3D>
#include <QPixmap>

class CameraTool : public QWidget, public Ui_CameraTool
{
    Q_OBJECT

public:
    explicit CameraTool(QWidget *parent = nullptr);
    //~CameraTool();

    void setData(float speed, QVector3D pos);
    void setFPS(QString str);

    float getSpeed(){return float(speedBox->value());}
    QVector3D getCameraPos(){return QVector3D(float(xBox->value()), float(yBox->value()), float(zBox->value()));}

protected:
        virtual void wheelEvent(QWheelEvent *pEvent) override{Q_UNUSED(pEvent)}
        virtual void mousePressEvent(QMouseEvent *pEvent) override{Q_UNUSED(pEvent)}
        virtual void mouseMoveEvent(QMouseEvent *pEvent) override{Q_UNUSED(pEvent)}
        virtual void mouseReleaseEvent(QMouseEvent *pEvent) override{Q_UNUSED(pEvent)}
        virtual void mouseDoubleClickEvent(QMouseEvent *pEvent) override{Q_UNUSED(pEvent)}

        virtual void keyPressEvent(QKeyEvent *event) override{Q_UNUSED(event)}
        virtual void keyReleaseEvent(QKeyEvent *event) override{Q_UNUSED(event)}

protected slots:
    void onDataChange();

signals:
    void dataChange(float speed, QVector3D pos);
    void cameraRes();
private:
    QString m_styleSheet = QString("") + "QToolButton {\
                background-color: rgb(118, 118, 118);\
                color: rgb(255, 255, 255);\
                text-align: center;\
                border: none;\
                font-size: 12px;\
                min-height: 20px;\
                max-height: 20px;\
                min-width: 20px;\
                max-width: 20px;\
                \
            }\
            \
            QToolButton:hover {\
                background-color: rgb(99, 99, 99);\
                color: rgb(255, 255, 255);\
                text-align: center;\
                \
            }\
            \
            QToolButton:checked,\
                QToolButton:pressed {\
                background-color: rgb(55, 55, 55);\
                color: rgb(255, 255, 255);\
                text-align: center;\
            }";
};

#endif // CAMERATOOL_H
