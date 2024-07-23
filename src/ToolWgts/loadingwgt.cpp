#include "loadingwgt.h"

#include <QMovie>

LoadingWgt::LoadingWgt(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    initialize();
}

LoadingWgt::~LoadingWgt()
{
    m_pMovie->stop();
}

void LoadingWgt::onSetTip(const QString &tip)
{
    m_pLabTip->setText(tip);
}

void LoadingWgt::initialize()
{
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_ShowModal,true);
    this->setStyleSheet("background-color: rgb(51,51,51);\
                         color: rgb(255, 255, 255);");
    setAttribute(Qt::WA_StyledBackground);
    m_pMovie = new QMovie(":/res/gif/loading.gif");
    m_pLabGIF->setMovie(m_pMovie);
    m_pLabGIF->setScaledContents(true);

    m_pMovie->start();
}
