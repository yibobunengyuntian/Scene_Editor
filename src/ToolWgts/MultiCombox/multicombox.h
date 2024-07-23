#ifndef MULTICOMBOX_H
#define MULTICOMBOX_H

#include <QWidget>
#include <QComboBox>

class MenuItem;
class QLineEdit;
class QListView;
class QStandardItemModel;

class MultiComBox : public QComboBox
{
    Q_OBJECT

public:
    MultiComBox(QWidget *parent = NULL);
    ~MultiComBox();

    void setText(const QString &text);
    //添加Iiem
    void AddItems(QList<MenuItem*> menus);

    //移除Item
    void RemoveItem(int row);

    void Clear();

    QStringList GetItemsText();

    QList<int> GetItemsId();

signals:
    void sigActivated(int row);

protected:
    void hidePopup() override;

    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    QList<MenuItem>       m_Menus;
    QLineEdit           *m_pLineEdit;
    QListView           *m_pListView;
    QStandardItemModel  *m_pItemModel;
    QString m_text = "";

private:
    void UpdateText();

public slots:
    void sltActivated(int row);
};

#endif // MULTICOMBOX_H
