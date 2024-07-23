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

    virtual void dropEvent(QDropEvent *event) override;								//���¶���
    virtual void dragEnterEvent(QDragEnterEvent *event) override;					//�е����봰�ڶ���
    virtual void dragMoveEvent(QDragMoveEvent *event) override;						//���������ڴ����ƶ�
    virtual void dragLeaveEvent(QDragLeaveEvent *event) override;					//������û���ͷ�
};

#endif // OPERATORDEFUALT_H
