#include "imageitem.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPointF>
#include <QGraphicsSceneDragDropEvent>
#include <QDrag>
#include <math.h>

ImageItem::ImageItem(QPixmap *pixmap)
{
    m_pix = *pixmap;
    setAcceptDrops(true);//If enabled is true, this item will accept hover events; otherwise, it will ignore them. By default, items do not accept hover events.
    m_scaleValue = 0;
    m_scaleDafault = 0;
    m_isMove = false;
}

QRectF ImageItem::boundingRect() const
{
    return QRectF(-m_pix.width() / 2, -m_pix.height() / 2,
                  m_pix.width(), m_pix.height());
}

void ImageItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                    QWidget *)
{
    painter->drawPixmap(-m_pix.width() / 2, -m_pix.height() / 2, m_pix);
}

void ImageItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()== Qt::LeftButton)
    {
        m_startPos = event->pos();//������ʱ����ȡ��ǰ�����ͼƬ�е����꣬
        m_isMove = true;//���������������
    }
    else if(event->button() == Qt::RightButton)
    {
        ResetItemPos();//�һ�������ô�С
    }

}

void ImageItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_isMove)
    {
        QPointF point = (event->pos() - m_startPos)*m_scaleValue;
        moveBy(point.x(), point.y());
    }
}

void ImageItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
    m_isMove = false;//����������Ѿ�̧��
}


void ImageItem::wheelEvent(QGraphicsSceneWheelEvent *event)//�������¼�
{
    if((event->delta() > 0)&&(m_scaleValue >= 50))//���Ŵ�ԭʼͼ���50��
    {
        return;
    }
    else if((event->delta() < 0)&&(m_scaleValue <= m_scaleDafault))//ͼ����С������Ӧ��С֮��Ͳ�������С
    {
        ResetItemPos();//����ͼƬ��С��λ�ã�ʹ֮����Ӧ�ؼ����ڴ�С
    }
    else
    {
        qreal qrealOriginScale = m_scaleValue;
        if(event->delta() > 0)//��������ǰ����
        {
            m_scaleValue*=1.1;//ÿ�ηŴ�10%
        }
        else
        {
            m_scaleValue*=0.9;//ÿ����С10%
        }
        setScale(m_scaleValue);
        if(event->delta() > 0)
        {
            moveBy(-event->pos().x()*qrealOriginScale*0.1, -event->pos().y()*qrealOriginScale*0.1);//ʹͼƬ���ŵ�Ч��������������������ڵ�Ϊ���Ľ������ŵ�
        }
        else
        {
            moveBy(event->pos().x()*qrealOriginScale*0.1, event->pos().y()*qrealOriginScale*0.1);//ʹͼƬ���ŵ�Ч��������������������ڵ�Ϊ���Ľ������ŵ�
        }
    }
}

void ImageItem::setQGraphicsViewWH(int nwidth, int nheight)//��������Ŀؼ�QGraphicsView��width��height���������У�������ͼ��ĳ���Ϳؼ��ĳ���ı�����ʹͼƬ���ŵ��ʺϿؼ��Ĵ�С
{
    int nImgWidth = m_pix.width();
    int nImgHeight = m_pix.height();
    qreal temp1 = nwidth*1.0/nImgWidth;
    qreal temp2 = nheight*1.0/nImgHeight;
    if(temp1>temp2)
    {
        m_scaleDafault = temp2;
    }
    else
    {
        m_scaleDafault = temp1;
    }
    setScale(m_scaleDafault);
    m_scaleValue = m_scaleDafault;
}

void ImageItem::ResetItemPos()//����ͼƬλ��
{
    m_scaleValue = m_scaleDafault;//���ű����ص�һ��ʼ������Ӧ����
    setScale(m_scaleDafault);//���ŵ�һ��ʼ������Ӧ��С
    setPos(0,0);
}

qreal ImageItem::getScaleValue() const
{
    return m_scaleValue;
}

