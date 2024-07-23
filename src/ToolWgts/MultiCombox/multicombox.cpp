#include "MultiComBox.h"
#include "MenuItem.h"
#include <QLineEdit>
#include <QListView>
#include <QStandardItemModel>
#include <QMouseEvent>

MultiComBox::MultiComBox(QWidget *parent)
    : QComboBox(parent)
{
    //Ϊ ComBox ���ñ༭��
    m_pLineEdit = new QLineEdit(this);
    m_pLineEdit->setReadOnly(true);
    m_pLineEdit->setEnabled(false);//���ɱ༭������ѡ���ı�
    this->setLineEdit(m_pLineEdit);
    m_pLineEdit->installEventFilter(this);
//    m_pLineEdit->setContextMenuPolicy(Qt::NoContextMenu);
    setContextMenuPolicy(Qt::NoContextMenu);

    //���� ComBox ��������
    m_pListView = new QListView(this);
    m_pListView->installEventFilter(this);
    this->setView(m_pListView);

    //���� ComBox ����ģ��
    m_pItemModel = new QStandardItemModel(this);
    this->setModel(m_pItemModel);

    connect(this, SIGNAL(activated(int)), this, SLOT(sltActivated(int)));
}

MultiComBox::~MultiComBox()
{

}

void MultiComBox::setText(const QString &text)
{
    m_text = text;
    m_pLineEdit->setText(m_text);
}

//*************************************************
//Function:        AddItems
//Description:     ���������˵�����
//Input:           menus���˵������б�
//Return:          ����;ִ��ʧ�ܣ��򷵻�false���жϲ��������򣬷����棬������������
//*************************************************
void MultiComBox::AddItems(QList<MenuItem*> menus)
{
    QStandardItem *pNewItem;
    foreach(MenuItem *pMenuItem, menus) {
        pNewItem = new QStandardItem(pMenuItem->GetMenuName());
        pNewItem->setCheckState(pMenuItem->GetIsChecked() ? Qt::Checked : Qt::Unchecked);
        pNewItem->setData(pMenuItem->GetMenuId());
        m_pItemModel->appendRow(pNewItem);
    }
    UpdateText();
}

void MultiComBox::RemoveItem(int row)
{
    m_pItemModel->removeRow(row);
    UpdateText();
}

void MultiComBox::Clear()
{
    m_pItemModel->clear();
    UpdateText();
}

QStringList MultiComBox::GetItemsText()
{
    QStringList strs;
    QString str = m_pLineEdit->text();
    if (!str.isEmpty()) {
        strs = str.split(";");
    }
    return strs;
}

QList<int> MultiComBox::GetItemsId()
{
    QList<int> ids;
    for (int i = 0; i < m_pItemModel->rowCount(); i++) {
        QStandardItem *item = m_pItemModel->item(i);
        if (item->checkState() == Qt::Checked) {
            ids << item->data().toInt();
        }
    }
    return ids;
}

//*************************************************
//Function:        hidePopup
//Description:     ���������Ӧλ�ã��ж��Ƿ����������˵�
//*************************************************
void MultiComBox::hidePopup()
{
    int width = this->view()->width();
    int height = this->view()->height();
    int x = QCursor::pos().x() - mapToGlobal(geometry().topLeft()).x() + geometry().x();
    int y = QCursor::pos().y() - mapToGlobal(geometry().topLeft()).y() + geometry().y();

    QRect rectView(0, this->height(), width, height);
    if (!rectView.contains(x, y)) {
        QComboBox::hidePopup();
    }
}

bool MultiComBox::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            if(watched== m_pLineEdit)
            {
                showPopup();

                return QComboBox::eventFilter(watched, event);
            }
            QListView *listView = qobject_cast<QListView*>(watched);
            if (NULL != listView) {
                int row = listView->currentIndex().row();
                sltActivated(row);
            }
            return true;
        }
    }


    return QComboBox::eventFilter(watched, event);
}

//*************************************************
//Function:        UpdateText
//Description:     ����lineedit�ı�
//*************************************************
void MultiComBox::UpdateText()
{
    if(m_text != "")
    {
        m_pLineEdit->setText(m_text);
    }
    else
    {
        QStringList strs;
        for (int i = 0; i < m_pItemModel->rowCount(); i++) {
            QStandardItem *item = m_pItemModel->item(i);
            if (item->checkState() == Qt::Checked) {
                strs << item->text();
            }
        }
        m_pLineEdit->setText(strs.join(";"));
        m_pLineEdit->setToolTip(strs.join("\n"));
    }
}

void MultiComBox::sltActivated(int row)
{
    QStandardItem *item = m_pItemModel->item(row);
    Qt::CheckState checkState = item->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked;
    item->setCheckState(checkState);
    UpdateText();

    emit sigActivated(row);
}