#include "checkboxmenu.h"
#include "ui_checkboxmenu.h"

#include <QDebug>

CheckBoxMenu::CheckBoxMenu(QToolButton *bindBtn, QWidget *parent) :
    QWidget(parent), m_pBindBtn(bindBtn)
{
    setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    //m_pVBox = new QVBoxLayout();
    //m_pVBox.set
    //boxsWgt->setLayout(m_pVBox);
}

void CheckBoxMenu::addWidget(QWidget *widget, QString name)
{
    if(widget == nullptr)
    {
        return;
    }
    QCheckBox *checkBox = new QCheckBox(name);
    checkBox->setChecked(true);
    m_pVBox->addWidget(checkBox);
    connect(checkBox, &QCheckBox::clicked, this, [widget](bool is){
        widget->setHidden(!is);
    });
    connect(allCheckBox, &QCheckBox::clicked, this, [checkBox](bool is){
        checkBox->setChecked(is);
        checkBox->clicked(is);
    });
}

void CheckBoxMenu::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event)
    if(m_pBindBtn)
    {
        m_pBindBtn->setDown(false);
    }
}
//bool CheckBoxMenu::eventFilter(QObject *obj, QEvent *event)
//{
//    qDebug() << m_pBindBtn->isDown();
//    if (Q_NULLPTR == obj) {
//        return false;
//    }
//    if (QEvent::ActivationChange == event->type()) {
//        if(QApplication::activeWindow() != this && QApplication::activeWindow() == parentWidget()){
//            qDebug() << m_pBindBtn->isDown();
//            if(m_pBindBtn)
//            {
//                m_pBindBtn->setDown(false);
//            }
//            this->hide();
//        }
//    }

//    return QWidget::eventFilter(obj, event);
//}
