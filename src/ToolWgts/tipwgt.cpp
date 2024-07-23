#include "tipwgt.h"
#include "ui_tipwgt.h"

TipWgt::TipWgt(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);//�޴���
    setAttribute(Qt::WA_TranslucentBackground);//����͸��
}

void TipWgt::setText(const QString &text)
{
    label->setText(text);
}
