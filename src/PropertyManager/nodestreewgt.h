#ifndef NODESTREEWGT_H
#define NODESTREEWGT_H

#include "ui_nodestreewgt.h"
#include <QWidget>
#include <QStandardItemModel>
#include "Nodes/node.h"

class NodesTreeWgt : public QWidget, public Ui_NodesTreeWgt
{
    Q_OBJECT

public:
    explicit NodesTreeWgt(QWidget *parent = nullptr);
    ~NodesTreeWgt();

signals:
    //void selectNodeSignal(Node *node);

public slots:
    void addNode(Node *node, Node *parent = nullptr);
    void delNode(Node *);
    void selectNode(Node *);
    void removeAll();

protected slots:
    void treeItemChanged(QStandardItem* item);
    void onTreeViewClicked(const QModelIndex &index);

protected:
    bool isRepetitionName(const QString name, const Node *parent = nullptr);

private:
    QMap<int,QIcon> m_iconMap;
    QMap<QStandardItem*, Node *> m_nodeMap;
    QStandardItemModel* m_pStandardItemModel = nullptr;
    QStandardItem *m_pCurrStandardItem = nullptr;
};

#endif // NODESTREEWGT_H
