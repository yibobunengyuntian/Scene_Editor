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

    virtual void dropEvent(QDropEvent *event) = 0;								//���¶���
    virtual void dragEnterEvent(QDragEnterEvent *event) = 0;					//�е����봰�ڶ���
    virtual void dragMoveEvent(QDragMoveEvent *event) = 0;						//���������ڴ����ƶ�
    virtual void dragLeaveEvent(QDragLeaveEvent *event) = 0;					//������û���ͷ�

protected:
    RenderWgt *m_pRenderWgt = nullptr;
};

#endif // OPERATORBASE_H
