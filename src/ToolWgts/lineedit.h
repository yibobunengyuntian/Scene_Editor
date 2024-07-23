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
    void dropEvent(QDropEvent *event) override;								//放下动作
    void dragEnterEvent(QDragEnterEvent *event) override;					//托到进入窗口动作
    void dragMoveEvent(QDragMoveEvent *event) override;						//拖着物体在窗口移动
    void dragLeaveEvent(QDragLeaveEvent *event) override;					//拖走了没有释放
};

#endif // LINEEDIT_H
