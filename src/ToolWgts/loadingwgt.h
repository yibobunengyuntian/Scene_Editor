#ifndef LOADINGWGT_H
#define LOADINGWGT_H

#include "ui_loadingwgt.h"

#include <QWidget>

class QMovie;

class LoadingWgt : public QWidget, public Ui_LoadingWgt
{
    Q_OBJECT

public:
    explicit LoadingWgt(QWidget *parent = nullptr);
    ~LoadingWgt();

public slots:
    void onSetTip(const QString &tip);

//protected:
    void initialize();

private:
    QMovie *m_pMovie = nullptr;
};

#endif // LOADINGWGT_H
