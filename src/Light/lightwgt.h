#ifndef LIGHTWGT_H
#define LIGHTWGT_H

#include <QWidget>
#include "ui_lightwgt.h"
#include <QToolButton>
#include <QVector3D>

class LightWgt : public QWidget, public Ui_LightWgt
{
    Q_OBJECT

public:
    explicit LightWgt(QToolButton *bindBtn, QWidget *parent = nullptr);
    ~LightWgt();

    QVector3D getLightDir(){ return m_lightDir;}

protected:
    void hideEvent(QHideEvent *event) override;

signals:
    void lightDirChanged(QVector3D dir);
    void isUseCameraDir(bool is);

protected slots:
    void dirCheckBox_x_changed(double x);
    void dirCheckBox_y_changed(double y);
    void dirCheckBox_z_changed(double z);
    void camDirCheckBoxChanged(bool is);

private:
    QToolButton *m_pBindBtn = nullptr;

    QVector3D m_lightDir = QVector3D(-0.2f, -1.0f, -0.3f);
    QVector3D m_lightColor = QVector3D(0, 0.4f, 0.6f);

    QString m_styleSheet = QString("") + "QToolButton {\
                background-color: rgb(118, 118, 118);\
                color: rgb(255, 255, 255);\
                text-align: center;\
                border: none;\
                font-size: 12px;\
                min-height: 15px;\
                max-height: 15px;\
                min-width: 15px;\
                max-width: 15px;\
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

#endif // LIGHTWGT_H
