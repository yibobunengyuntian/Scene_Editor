#include "startwgt.h"
#include<QGraphicsDropShadowEffect>
#include<QTimer>
#include <QThread>
#include <QMovie>
#include "../../mainwgt.h"

StartWgt::StartWgt(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &StartWgt::updateBarValue);
    m_timer->start(30);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    //设置边框阴影
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);
    shadow->setColor(QColor(0, 0, 0, 60));
    shadow->setBlurRadius(10);
    frame->setGraphicsEffect(shadow);
    m_value = 0;
    progressBar->setValue(0);

    setAttribute(Qt::WA_StyledBackground);
    m_pMovie = new QMovie(":/res/gif/earth_rotate.gif");
    label_gif->setMovie(m_pMovie);
    label_gif->setScaledContents(true);

    m_pMovie->start();
}

StartWgt::~StartWgt()
{

}

void StartWgt::setBarValue(int value)
{
    m_value = value;
    progressBar->setValue(m_value);
}

void StartWgt::updateBarValue()
{
    m_value++;
    progressBar->setValue(m_value);
    if (m_value >= 99)
    {
        QWidget *win = new MainWgt();
        progressBar->setValue(100);
        QThread::msleep(200);
        win->show();
        m_pMovie->stop();
        m_timer->stop();
        this->close();
    }
}
