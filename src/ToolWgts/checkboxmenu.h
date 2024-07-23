#ifndef CHECKBOXMENU_H
#define CHECKBOXMENU_H

#include <QWidget>
#include "titebar.h"
#include "ui_checkboxmenu.h"

class CheckBoxMenu : public QWidget, public Ui_CheckBoxMenu
{
    Q_OBJECT

public:
    explicit CheckBoxMenu(QToolButton *bindBtn, QWidget *parent = nullptr);

    void addWidget(QWidget *widget, QString name);

protected:
//    bool eventFilter(QObject *obj, QEvent *event);

    void hideEvent(QHideEvent *event) override;

private:
    QToolButton *m_pBindBtn = nullptr;

};

#endif // CHECKBOXMENU_H
