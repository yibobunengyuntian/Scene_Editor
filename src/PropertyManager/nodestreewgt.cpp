#include "nodestreewgt.h"
#include "ui_nodestreewgt.h"
#include "propertymanager.h"

NodesTreeWgt::NodesTreeWgt(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    m_pStandardItemModel = new QStandardItemModel(treeView);
    treeView->setModel(m_pStandardItemModel);
    m_iconMap[0] = QIcon(":/res/icon/model_icon.png");
    m_iconMap[1] = QIcon(":/res/icon/mesh_icon.png");
    connect(m_pStandardItemModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(treeItemChanged(QStandardItem*)));
    connect(treeView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onTreeViewClicked(const QModelIndex &)));
}

NodesTreeWgt::~NodesTreeWgt()
{
}

void NodesTreeWgt::addNode(Node *node, Node *parent)
{
    if(node == nullptr)
    {
        return;
    }
    QString name = node->name();
    int index = 1;
    while(isRepetitionName(name, parent))
    {
        name = node->name() + "_" + QString::number(index);
        index ++;
    }
    node->setName(name);
    QStandardItem* itemProject;
    itemProject = new QStandardItem(m_iconMap[node->type()],node->name());
    itemProject->setCheckable(true);
    itemProject->setCheckState(Qt::Checked);
    m_nodeMap[itemProject] = node;

    if(parent == nullptr)
    {
        m_pStandardItemModel->appendRow(itemProject);
    }
    else
    {
        QStandardItem* parentItem = m_nodeMap.key(parent);
        parentItem->appendRow(itemProject);
    }
    foreach(auto child, node->childs())
    {
        addNode(child, node);
    }
}

void NodesTreeWgt::delNode(Node *node)
{
    if(node == nullptr)
    {
        return;
    }
    foreach(auto child, node->childs())
    {
        delNode(child);
    }
    QStandardItem *item = m_nodeMap.key(node);
    if(item)
    {
        if(item->parent())
        {
            item->parent()->removeRow(item->index().row());
        }
        else
        {
            m_pStandardItemModel->removeRow(item->index().row());
        }
        m_nodeMap.remove(item);
        treeView->setCurrentIndex(QModelIndex());
        m_pCurrStandardItem = nullptr;
    }
}

void NodesTreeWgt::selectNode(Node *node)
{
    QStandardItem *item = m_nodeMap.key(node);
    if(item)
    {
        QModelIndex modelindex = item->index();
        treeView->setCurrentIndex(modelindex);
        m_pCurrStandardItem = item;
    }
    else
    {
        treeView->setCurrentIndex(QModelIndex());
        m_pCurrStandardItem = nullptr;
    }
}

void NodesTreeWgt::removeAll()
{
    m_pStandardItemModel->clear();
    m_nodeMap.clear();
}

void NodesTreeWgt::treeItemChanged(QStandardItem *item)
{
    if(nullptr == m_nodeMap[item])
    {
        return;
    }
    if(!isRepetitionName(item->text(), m_nodeMap[item]->parent()))
    {
        m_nodeMap[item]->setName(item->text());
    }
    m_nodeMap[item]->setHide(!item->checkState());
}

void NodesTreeWgt::onTreeViewClicked(const QModelIndex &index)
{
    QStandardItem * item = m_pStandardItemModel->itemFromIndex(index);
    if(m_pCurrStandardItem)
    {
        m_pCurrStandardItem->setText(m_nodeMap[m_pCurrStandardItem]->name());
    }
    if(treeView->currentIndex() == index)
    {
        if(m_pCurrStandardItem == item)
        {
            return;
        }
        else
        {
            m_pCurrStandardItem = item;
            g_PropertyManager->selectNode(m_nodeMap[m_pCurrStandardItem]);
        }
    }
}

bool NodesTreeWgt::isRepetitionName(const QString name, const Node *parent)
{
    foreach(auto it, m_nodeMap)
    {
        if(it->parent() == parent && it->name() == name)
        {
            return true;
        }
    }

    return false;
}
