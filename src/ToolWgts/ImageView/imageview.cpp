#include "imageview.h"

ImageView::ImageView(QWidget *parent) :
    QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    setupUi(this);
}

ImageView::~ImageView()
{

}

void ImageView::recvShowPic(QImage image)
{
    if(m_pImage)
    {
        ImageGraphic->scene()->removeItem(m_pImage);
        delete []m_pImage;
        m_pImage = nullptr;
    }
    QPixmap ConvertPixmap=QPixmap::fromImage(image);//The QPixmap class is an off-screen image representation that can be used as a paint device
    QGraphicsScene  *qgraphicsScene = new QGraphicsScene;//Ҫ��QGraphicsView�ͱ���Ҫ��QGraphicsScene��������
    m_pImage = new ImageItem(&ConvertPixmap);//ʵ������ImageItem�Ķ���m_Image������̳���QGraphicsItem�����Լ�д����
    int nwith = ImageGraphic->width();//��ȡ����ؼ�Graphics View�Ŀ��
    int nheight = ImageGraphic->height();//��ȡ����ؼ�Graphics View�ĸ߶�
    m_pImage->setQGraphicsViewWH(nwith,nheight);//������ؼ�Graphics View��width��height������m_Image��
    qgraphicsScene->addItem(m_pImage);//��QGraphicsItem�����Ž�QGraphicsScene��
    ImageGraphic->setSceneRect(QRectF(-(nwith/2),-(nheight/2),nwith,nheight));//ʹ�Ӵ��Ĵ�С�̶���ԭʼ��С��������ͼƬ�ķŴ���Ŵ�Ĭ��״̬��ͼƬ�Ŵ��ʱ���Ӵ����߻��Զ����ֹ������������Ӵ��ڵ���Ұ���󣩣���ֹͼƬ�Ŵ��������С��ʱ���Ӵ�̫���������۲�ͼƬ
    ImageGraphic->setScene(qgraphicsScene);//Sets the current scene to scene. If scene is already being viewed, this function does nothing.
    ImageGraphic->setFocus();//������Ľ������õ���ǰGraphics View�ؼ�
}
