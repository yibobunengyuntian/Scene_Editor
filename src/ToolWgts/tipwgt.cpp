#include "tipwgt.h"
#include "ui_tipwgt.h"

TipWgt::TipWgt(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);//ÎÞ´°Ìå
    setAttribute(Qt::WA_TranslucentBackground);//±³¾°Í¸Ã÷
}

void TipWgt::setText(const QString &text)
{
    label->setText(text);
}
