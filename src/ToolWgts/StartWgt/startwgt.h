#ifndef STARTWGT_H
#define STARTWGT_H

#include <QWidget>
#include "ui_startwgt.h"

class StartWgt : public QWidget, public Ui_StartWgt
{
    Q_OBJECT

public:
    explicit StartWgt(QWidget *parent = nullptr);
    ~StartWgt();

    void setBarValue(int value);
private slots:
    void updateBarValue();

private:
    QMovie *m_pMovie;
    QTimer *m_timer;
    int m_value;
};

#endif // STARTWGT_H
