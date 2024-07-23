#ifndef NEWPROJECTWGT_H
#define NEWPROJECTWGT_H

#include "ui_newprojectwgt.h"
#include <QDir>
#include <QFileDialog>
#include "titebar.h"

class NewProjectWgt : public QWidget,public Ui_NewProjectWgt
{
    Q_OBJECT

public:
    explicit NewProjectWgt(QWidget *parent = nullptr);
    ~NewProjectWgt();

    QString getDirectory();

private slots:
    void quitEvent();
    void mid_maxEvent();
    void minEvent();
    void aboutEvent();
    void configEvent();
    void personEvent();
    void moveWindowEvent(QPoint& pos);

protected slots:
    void onOK();
    void onBrowse();

    void onNameChanged(QString name);
protected:
    void initialize();
private:
    QString m_directory;
};

#endif // NEWPROJECTWGT_H
