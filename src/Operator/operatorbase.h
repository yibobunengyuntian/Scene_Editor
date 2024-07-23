#ifndef OPERATORBASE_H
#define OPERATORBASE_H

#include <QObject>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include "renderwgt.h"

class OperatorBase: public QObject
{
    Q_OBJECT
public:
    OperatorBase(RenderWgt *renderWgt);

public:
    virtual void wheelEvent(QWheelEvent *pEvent) = 0;
    virtual void mousePressEvent(QMouseEvent *pEvent) = 0;
    virtual void mouseMoveEvent(QMouseEvent *pEvent) = 0;
    virtual void mouseReleaseEvent(QMouseEvent *pEvent) = 0;
    virtual void mouseDoubleClickEvent(QMouseEvent *pEvent) = 0;

    virtual void keyPressEvent(QKeyEvent *event) = 0;
    virtual void keyReleaseEvent(QKeyEvent *event) = 0;

    virtual void dropEvent(QDropEvent *event) = 0;								//放下动作
    virtual void dragEnterEvent(QDragEnterEvent *event) = 0;					//托到进入窗口动作
    virtual void dragMoveEvent(QDragMoveEvent *event) = 0;						//拖着物体在窗口移动
    virtual void dragLeaveEvent(QDragLeaveEvent *event) = 0;					//拖走了没有释放

protected:
    RenderWgt *m_pRenderWgt = nullptr;
};

#endif // OPERATORBASE_H
