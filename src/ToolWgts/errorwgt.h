#ifndef ERRORWGT_H
#define ERRORWGT_H

#include <QWidget>
#include "ui_errorwgt.h"

class ErrorWgt : public QWidget, public Ui_ErrorWgt
{
    Q_OBJECT

public:
    explicit ErrorWgt(QWidget *parent = nullptr);
    ~ErrorWgt();

private:

};

#endif // ERRORWGT_H
