#include "tipwgt.h"
#include "ui_tipwgt.h"

TipWgt::TipWgt(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);//无窗体
    setAttribute(Qt::WA_TranslucentBackground);//背景透明
}

void TipWgt::setText(const QString &text)
{
    label->setText(text);
}
