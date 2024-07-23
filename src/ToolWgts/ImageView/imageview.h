#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QWidget>
#include "ui_imageview.h"
#include "imageitem.h"
class ImageView : public QWidget, public Ui_ImageView
{
    Q_OBJECT

public:
    explicit ImageView(QWidget *parent = nullptr);
    ~ImageView();

public slots:
    void recvShowPic(QImage image);//接收并显示图片的函数

private:
    ImageItem *m_pImage = nullptr;
};

#endif // IMAGEVIEW_H
