#include "cameratool.h"
#include "ui_cameratool.h"
#include <QDebug>

CameraTool::CameraTool(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    move(0, 0);
    setWindowFlags(Qt::FramelessWindowHint);//无窗体
    setAttribute(Qt::WA_TranslucentBackground);//背景透明

    speedBox->setContextMenuPolicy(Qt::NoContextMenu);
    xBox->setContextMenuPolicy(Qt::NoContextMenu);
    yBox->setContextMenuPolicy(Qt::NoContextMenu);
    zBox->setContextMenuPolicy(Qt::NoContextMenu);

    speedBox->setToolTip(("相机移动速度"));
    xBox->setToolTip(("相机位置 X 坐标"));
    yBox->setToolTip(("相机位置 Y 坐标"));
    zBox->setToolTip(("相机位置 Z 坐标"));
    resBtn->setStyleSheet(m_styleSheet);
    resBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    resBtn->setIcon(QIcon(":/res/icon/res.png"));
    resBtn->setToolTip(("相机复位"));

    QPixmap pixmap;
    pixmap.load(":/res/icon/running.png");
    pixmap.scaled(label->size()/2, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    label->setScaledContents(true);
    label->setPixmap(pixmap);

    connect(speedBox, SIGNAL(valueChanged(double)),this,SLOT(onDataChange()));
    connect(xBox, SIGNAL(valueChanged(double)),this,SLOT(onDataChange()));
    connect(yBox, SIGNAL(valueChanged(double)),this,SLOT(onDataChange()));
    connect(zBox, SIGNAL(valueChanged(double)),this,SLOT(onDataChange()));
    connect(resBtn,&QToolButton::clicked,[=](){
        emit cameraRes();
    });
}


void CameraTool::setData(float speed, QVector3D pos)
{
    disconnect(speedBox, SIGNAL(valueChanged(double)),this,SLOT(onDataChange()));
    disconnect(xBox, SIGNAL(valueChanged(double)),this,SLOT(onDataChange()));
    disconnect(yBox, SIGNAL(valueChanged(double)),this,SLOT(onDataChange()));
    disconnect(zBox, SIGNAL(valueChanged(double)),this,SLOT(onDataChange()));

    speedBox->setValue(double(speed));
    xBox->setValue(double(pos.x()));
    yBox->setValue(double(pos.y()));
    zBox->setValue(double(pos.z()));

    connect(speedBox, SIGNAL(valueChanged(double)),this,SLOT(onDataChange()));
    connect(xBox, SIGNAL(valueChanged(double)),this,SLOT(onDataChange()));
    connect(yBox, SIGNAL(valueChanged(double)),this,SLOT(onDataChange()));
    connect(zBox, SIGNAL(valueChanged(double)),this,SLOT(onDataChange()));
}

void CameraTool::setFPS(QString str)
{
    fpsLabel->setText(str);
}

void CameraTool::onDataChange()
{
    emit dataChange(float(speedBox->value()), QVector3D(float(xBox->value()), float(yBox->value()), float(zBox->value())));
}
