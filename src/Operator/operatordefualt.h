#ifndef OPERATORDEFUALT_H
#define OPERATORDEFUALT_H

#include "operatorbase.h"

class OperatorDefualt: public OperatorBase
{
public:
    OperatorDefualt(RenderWgt* renderWgt);

public:
    virtual void wheelEvent(QWheelEvent *pEvent) override;
    virtual void mousePressEvent(QMouseEvent *pEvent) override;
    virtual void mouseMoveEvent(QMouseEvent *pEvent) override;
    virtual void mouseReleaseEvent(QMouseEvent *pEvent) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *pEvent) override;

    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;

    virtual void dropEvent(QDropEvent *event) override;								//放下动作
    virtual void dragEnterEvent(QDragEnterEvent *event) override;					//托到进入窗口动作
    virtual void dragMoveEvent(QDragMoveEvent *event) override;						//拖着物体在窗口移动
    virtual void dragLeaveEvent(QDragLeaveEvent *event) override;					//拖走了没有释放
};

#endif // OPERATORDEFUALT_H
