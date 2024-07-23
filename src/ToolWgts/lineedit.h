#ifndef LINEEDIT_H
#define LINEEDIT_H
#include <QDropEvent>
#include <QLineEdit>

class LineEdit: public QLineEdit
{
    Q_OBJECT
public:
    LineEdit(QWidget *parent = nullptr);

protected:
    void dropEvent(QDropEvent *event) override;								//���¶���
    void dragEnterEvent(QDragEnterEvent *event) override;					//�е����봰�ڶ���
    void dragMoveEvent(QDragMoveEvent *event) override;						//���������ڴ����ƶ�
    void dragLeaveEvent(QDragLeaveEvent *event) override;					//������û���ͷ�
};

#endif // LINEEDIT_H
