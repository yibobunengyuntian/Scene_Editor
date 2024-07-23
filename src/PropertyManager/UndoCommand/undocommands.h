#ifndef UNDOCOMMANDS_H
#define UNDOCOMMANDS_H

#include <QUndoCommand>
#include <QVariantMap>
#include "../propertymanager.h"

class UndoCommandBase : public QObject,public QUndoCommand
{
    Q_OBJECT
public:
    explicit UndoCommandBase(const QString &text, QUndoCommand *parent = nullptr);
    ~UndoCommandBase();

    virtual void redo() override;
    virtual void undo() override;

private:


};

class UndoCommandAddNode : public UndoCommandBase
{
    Q_OBJECT
public:
    explicit UndoCommandAddNode(Node* node, const QString &text, QUndoCommand *parent = nullptr);
    ~UndoCommandAddNode();

    virtual void redo() override;
    virtual void undo() override;

private:
    Node *m_pNode = nullptr;

};

class UndoCommandDelNode : public UndoCommandBase
{
    Q_OBJECT
public:
    explicit UndoCommandDelNode(Node* node, const QString &text, QUndoCommand *parent = nullptr);

    virtual void redo() override;
    virtual void undo() override;

private:
    Node *m_pNode = nullptr;

};

class UndoCommandTraNode : public UndoCommandBase
{
    Q_OBJECT
public:
    explicit UndoCommandTraNode(Node* node, QVector3D tra, const QString &text, QUndoCommand *parent = nullptr);

    virtual void redo() override;
    virtual void undo() override;

private:
    Node *m_pNode = nullptr;
    QVector3D m_tra;
};

class UndoCommandRotNode : public UndoCommandBase
{
    Q_OBJECT
public:
    explicit UndoCommandRotNode(Node* node, QVector3D rot, const QString &text, QUndoCommand *parent = nullptr);

    virtual void redo() override;
    virtual void undo() override;

private:
    Node *m_pNode = nullptr;
    QVector3D m_rot;
};

class UndoCommandScaNode : public UndoCommandBase
{
    Q_OBJECT
public:
    explicit UndoCommandScaNode(Node* node, QVector3D sca, const QString &text, QUndoCommand *parent = nullptr);

    virtual void redo() override;
    virtual void undo() override;

private:
    Node *m_pNode = nullptr;
    QVector3D m_sca;
};

class UndoCommandChangeNodeMaterial : public UndoCommandBase
{
    Q_OBJECT
public:
    explicit UndoCommandChangeNodeMaterial(Node* node, Material material, const QString &text, QUndoCommand *parent = nullptr);

    virtual void redo() override;
    virtual void undo() override;

private:
    Node *m_pNode = nullptr;
    Material m_oldMaterial;
    Material m_newMaterial;
};

class UndoCommandChangeNodeName : public UndoCommandBase
{
    Q_OBJECT
public:
    explicit UndoCommandChangeNodeName(Node* node, QString name, const QString &text, QUndoCommand *parent = nullptr);

    virtual void redo() override;
    virtual void undo() override;

private:
    Node *m_pNode = nullptr;
    QString m_oldName;
    QString m_newName;
};

#endif // UNDOCOMMANDS_H
