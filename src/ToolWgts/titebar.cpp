#include "titebar.h"
#include "ui_titebar.h"

TiteBar::TiteBar(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    initialize();
}

TiteBar::~TiteBar()
{

}
void TiteBar::onCloseWgt()
{
    emit quitEvent();
}

void TiteBar::onMaxWgt()
{
    emit mid_maxEvent();
}

void TiteBar::onMinWgt()
{
    emit minEvent();
}

//void TiteBar::on_pb_about_clicked()
//{
//    emit aboutEvent();
//}

//void TiteBar::on_pb_config_clicked()
//{
//    emit configEvent();
//}

//void TiteBar::on_pb_person_clicked()
//{
//    emit personEvent();
//}

void TiteBar::initialize()
{
    setWindowFlags(Qt::FramelessWindowHint);//无边框
    vBox->setSpacing(3);
    m_pTiteIcon = new QLabel(this);
    vBox->addWidget(m_pTiteIcon);
    m_pTiteText = new QLabel(this);
    m_pTiteText->setAlignment(Qt::AlignCenter);           //设置字体居中显示
    // QFont font = QFont("宋体", 12);
    // m_pTiteText->setFont(font);                           //设置字体
   // m_pTiteText->setText("openGL_test");
    vBox->addWidget(m_pTiteText);
    vBox->addStretch();
    minimizeBtn = new QToolButton(this);
    minimizeBtn->setStyleSheet(QString("") + "QToolButton {\
                               background-color: transparent;\
                               color: rgb(255, 255, 255);\
                               text-align: center;\
                               border: none;\
                               font-size: 12px;\
                               min-height: 25px;\
                               max-height: 25px;\
                               min-width: 25px;\
                               max-width: 25px;\
                               \
                           }\
                           \
                           QToolButton:hover {\
                               background-color: rgb(77, 77, 77);\
                               color: rgb(255, 255, 255);\
                               text-align: center;\
                               \
                           }\
                           \
                           QToolButton:checked,\
                               QToolButton:pressed {\
                               background-color: rgb(99, 99, 99);\
                               color: rgb(255, 255, 255);\
                               text-align: center;\
                           }");
    minimizeBtn->setIcon(QIcon(":/res/icon/minimize.png"));
    vBox->addWidget(minimizeBtn);
    maximizeBtn = new QToolButton(this);
    maximizeBtn->setStyleSheet(QString("") + "QToolButton {\
                               background-color: transparent;\
                               color: rgb(255, 255, 255);\
                               text-align: center;\
                               border: none;\
                               font-size: 12px;\
                               min-height: 25px;\
                               max-height: 25px;\
                               min-width: 25px;\
                               max-width: 25px;\
                               \
                           }\
                           \
                           QToolButton:hover {\
                               background-color: rgb(77, 77, 77);\
                               color: rgb(255, 255, 255);\
                               text-align: center;\
                               \
                           }\
                           \
                           QToolButton:checked,\
                               QToolButton:pressed {\
                               background-color: rgb(99, 99, 99);\
                               color: rgb(255, 255, 255);\
                               text-align: center;\
                           }");
    maximizeBtn->setIcon(QIcon(":/res/icon/maximize.png"));
    vBox->addWidget(maximizeBtn);
    closeBtn = new QToolButton(this);
    closeBtn->setStyleSheet(m_styleSheet);
    closeBtn->setIcon(QIcon(":/res/icon/close.png"));
    vBox->addWidget(closeBtn);

    connect(minimizeBtn, &QToolButton::clicked, this, &TiteBar::onMinWgt);
    connect(maximizeBtn, &QToolButton::clicked, this, &TiteBar::onMaxWgt);
    connect(closeBtn, &QToolButton::clicked, this, &TiteBar::onCloseWgt);
}

void TiteBar::setMaxIcon(bool arg)
{
    QIcon icon;
    if(arg)
    {
        icon.addFile(":/res/images/btn_mid.png");
    }
    else {
       icon.addFile(":/res/images/btn_max.png");
    }

}

void TiteBar::setWindowTitle(QString _title)
{
    m_pTiteText->setText(_title);
}

void TiteBar::setIcon(QString icon)
{
    QPixmap titePixmap(icon);
    m_pTiteIcon->setPixmap(titePixmap);
}

void TiteBar::hideMid_maxAndMin()
{
    minimizeBtn->hide();
    maximizeBtn->hide();
}


void TiteBar::mousePressEvent(QMouseEvent *event)
{
    this->setFocus();
    if(Qt::LeftButton == event->button())
    {
        pLast=event->globalPos();
        // event->ignore();
        m_bPressed = true;
}
}

void TiteBar::mouseMoveEvent(QMouseEvent* event)
{
    if( m_bPressed)
    {
        QPoint ptemp = event->globalPos();
        ptemp = ptemp - pLast ;
        pLast = event->globalPos();
        emit moveWindow(ptemp);
    }
}

 void TiteBar::mouseReleaseEvent(QMouseEvent * event)
 {
     Q_UNUSED(event)
    m_bPressed = false;
 }

 void TiteBar::mouseDoubleClickEvent(QMouseEvent *event)
 {
     if(Qt::LeftButton == event->button())
     {
         if(!minimizeBtn->isHidden())
            emit mid_maxEvent();
     }
 }
