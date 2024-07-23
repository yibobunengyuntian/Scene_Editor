#include "undocommands.h"

UndoCommandBase::UndoCommandBase(const QString &text, QUndoCommand *parent):
    QUndoCommand(text,parent)
{

}

UndoCommandBase::~UndoCommandBase()
{

}

void UndoCommandBase::redo()
{

}

void UndoCommandBase::undo()
{

}

UndoCommandAddNode::UndoCommandAddNode(Node *node, const QString &text, QUndoCommand *parent):
    UndoCommandBase(text,parent)
{
    m_pNode = node;
}

UndoCommandAddNode::~UndoCommandAddNode()
{
    if(m_pNode)
    {
        delete []m_pNode;
        m_pNode = nullptr;
    }
}

void UndoCommandAddNode::redo()
{
    if(m_pNode == nullptr)
    {
        return;
    }
    g_PropertyManager->selectNode(nullptr);
    g_PropertyManager->addNode(m_pNode, m_pNode->parent());
}

void UndoCommandAddNode::undo()
{
    if(m_pNode == nullptr)
    {
        return;
    }
    g_PropertyManager->selectNode(nullptr);
    g_PropertyManager->delNode(m_pNode);
}


UndoCommandDelNode::UndoCommandDelNode(Node *node, const QString &text, QUndoCommand *parent):
    UndoCommandBase(text,parent)
{
    m_pNode = node;
}

void UndoCommandDelNode::redo()
{
    if(m_pNode == nullptr)
    {
        return;
    }
    g_PropertyManager->selectNode(nullptr);
    g_PropertyManager->delNode(m_pNode);
}

void UndoCommandDelNode::undo()
{
    if(m_pNode == nullptr)
    {
        return;
    }
    g_PropertyManager->selectNode(nullptr);
    g_PropertyManager->addNode(m_pNode, m_pNode->parent());
}

UndoCommandTraNode::UndoCommandTraNode(Node *node, QVector3D tra, const QString &text, QUndoCommand *parent):
    UndoCommandBase(text,parent)
{
    m_pNode = node;
    m_tra = tra;
}

void UndoCommandTraNode::redo()
{
    if(m_pNode == nullptr)
    {
        return;
    }
    m_pNode->translate(m_tra);
}

void UndoCommandTraNode::undo()
{
    if(m_pNode == nullptr)
    {
        return;
    }
    m_pNode->translate(-m_tra);
}

UndoCommandRotNode::UndoCommandRotNode(Node *node, QVector3D rot, const QString &text, QUndoCommand *parent):
    UndoCommandBase(text, parent)
{
    m_pNode = node;
    m_rot = rot;
}

void UndoCommandRotNode::redo()
{
    if(m_pNode == nullptr)
    {
        return;
    }
    m_pNode->rotate(m_rot);
}

void UndoCommandRotNode::undo()
{
    if(m_pNode == nullptr)
    {
        return;
    }
    m_pNode->rotate(-m_rot);
}

UndoCommandScaNode::UndoCommandScaNode(Node *node, QVector3D sca, const QString &text, QUndoCommand *parent):
    UndoCommandBase(text,parent)
{
    m_pNode = node;
    m_sca = sca;
}

void UndoCommandScaNode::redo()
{
    if(m_pNode == nullptr)
    {
        return;
    }
    m_pNode->scale(m_sca);
}

void UndoCommandScaNode::undo()
{
    if(m_pNode == nullptr)
    {
        return;
    }
    m_pNode->scale(QVector3D(1/m_sca.x(), 1/m_sca.y(), 1/m_sca.z()));
}

UndoCommandChangeNodeMaterial::UndoCommandChangeNodeMaterial(Node *node, Material material, const QString &text, QUndoCommand *parent):
    UndoCommandBase(text,parent)
{
    m_pNode = node;
    if(m_pNode == nullptr)
    {
        return;
    }
    m_oldMaterial = m_pNode->material();
    m_newMaterial = material;
}

void UndoCommandChangeNodeMaterial::redo()
{
    if(m_pNode == nullptr)
    {
        return;
    }
    m_pNode->setMaterial(m_newMaterial);
}

void UndoCommandChangeNodeMaterial::undo()
{
    if(m_pNode == nullptr)
    {
        return;
    }
    m_pNode->setMaterial(m_oldMaterial);
}

UndoCommandChangeNodeName::UndoCommandChangeNodeName(Node *node, QString name, const QString &text, QUndoCommand *parent):
    UndoCommandBase(text,parent)
{
    m_pNode = node;
    m_oldName = node->name();
    m_newName = name;
}

void UndoCommandChangeNodeName::redo()
{
    if(m_pNode == nullptr)
    {
        return;
    }
    m_pNode->setName(m_newName);
}

void UndoCommandChangeNodeName::undo()
{
    if(m_pNode == nullptr)
    {
        return;
    }
    m_pNode->setName(m_oldName);
}
