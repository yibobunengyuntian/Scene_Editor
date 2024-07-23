#ifndef COMPONENTBASE_H
#define COMPONENTBASE_H

#include "../propertymanager.h"

class ComponentBase: public QObject
{
    Q_OBJECT
public:
    ComponentBase();
    void bindNode(Node *node)
    {
        m_pBindNode = node;
    }

protected slots:
    virtual void run() = 0;

private slots:
    void updata();
protected:
    Node *m_pBindNode = nullptr;
    QVariantMap m_para;
};

#endif // COMPONENTBASE_H
