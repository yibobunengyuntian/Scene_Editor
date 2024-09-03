#include "imageview.h"

ImageView::ImageView(QWidget *parent) :
    QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    setupUi(this);
    QGraphicsScene  *qgraphicsScene = new QGraphicsScene;//要用QGraphicsView就必须要有QGraphicsScene搭配着用
    ImageGraphic->setScene(qgraphicsScene);//Sets the current scene to scene. If scene is already being viewed, this function does nothing.
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
    m_pImage = new ImageItem(&ConvertPixmap);//实例化类ImageItem的对象m_Image，该类继承自QGraphicsItem，是自己写的类
    int nwith = ImageGraphic->width();//获取界面控件Graphics View的宽度
    int nheight = ImageGraphic->height();//获取界面控件Graphics View的高度
    m_pImage->setQGraphicsViewWH(nwith,nheight);//将界面控件Graphics View的width和height传进类m_Image中
    ImageGraphic->scene()->addItem(m_pImage);//将QGraphicsItem类对象放进QGraphicsScene中
    ImageGraphic->setSceneRect(QRectF(-(nwith/2),-(nheight/2),nwith,nheight));//使视窗的大小固定在原始大小，不会随图片的放大而放大（默认状态下图片放大的时候视窗两边会自动出现滚动条，并且视窗内的视野会变大），防止图片放大后重新缩小的时候视窗太大而不方便观察图片
    ImageGraphic->setFocus();//将界面的焦点设置到当前Graphics View控件
}
