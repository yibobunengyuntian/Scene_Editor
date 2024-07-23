#ifndef SETWGT_H
#define SETWGT_H

#include <QWidget>
#include "ui_setwgt.h"

class SetWgt : public QWidget, public Ui_SetWgt
{
    Q_OBJECT

public:
    explicit SetWgt(QWidget *parent = nullptr);
    ~SetWgt();

private:

};

#endif // SETWGT_H
