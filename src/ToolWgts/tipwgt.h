#ifndef TIPWGT_H
#define TIPWGT_H

#include <QWidget>
#include "ui_tipwgt.h"

class TipWgt : public QWidget, public Ui_TipWgt
{
    Q_OBJECT

public:
    explicit TipWgt(QWidget *parent = nullptr);

    void setText(const QString &text);
};

#endif // TIPWGT_H
