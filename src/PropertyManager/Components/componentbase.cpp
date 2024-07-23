#include "componentbase.h"

ComponentBase::ComponentBase()
{
    connect(g_PropertyManager, SIGNAL(renderLogic()), this, SLOT(updata()));
}

void ComponentBase::updata()
{
    run();
    Node *node = g_PropertyManager->selectNode();
    if(m_pBindNode == nullptr || node == nullptr)
    {
        return;
    }
    if(node == m_pBindNode)
    {
        g_PropertyManager->nodeEditWgt()->update();
        return;
    }

    while(node != nullptr)
    {
        if(node == m_pBindNode)
        {
            g_PropertyManager->nodeEditWgt()->update();
            return;
        }
        node = node->parent();
    }
}
