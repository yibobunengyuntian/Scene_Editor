#ifndef COMPONENTEDITWGT_H
#define COMPONENTEDITWGT_H

#include <QWidget>
#include "ui_componenteditwgt.h"
#include "componentbase.h"

class ComponentEditWgt : public QWidget, public Ui_ComponentEditWgt
{
    Q_OBJECT

public:
    explicit ComponentEditWgt(QWidget *parent = nullptr);
    ~ComponentEditWgt();

    void bindComponent(ComponentBase *com);
private:
    ComponentBase *m_pBindComponent = nullptr;
};

#endif // COMPONENTEDITWGT_H
