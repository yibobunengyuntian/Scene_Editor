#include "lineedit.h"

LineEdit::LineEdit(QWidget *parent)
    :QLineEdit(parent)
{
    setAcceptDrops(true);  /* 允许拖放 */
    setFocusPolicy(Qt::ClickFocus);
    setFocusPolicy(Qt::StrongFocus);
}

void LineEdit::dropEvent(QDropEvent *event)
{

}

void LineEdit::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void LineEdit::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void LineEdit::dragLeaveEvent(QDragLeaveEvent *event)
{

}
