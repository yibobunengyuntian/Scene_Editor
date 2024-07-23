#include "propertyswgt.h"
#include "ui_propertyswgt.h"

PropertysWgt::PropertysWgt(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    listWidget->setViewMode(QListWidget::IconMode);//显示模式
    listWidget->setIconSize(QSize(50, 50));//设置图片大小
    listWidget->setSpacing(3);//间距
    listWidget->setResizeMode(QListView::Adjust);//适应布局调整
    listWidget->setMovement(QListView::Static);//不能移动
    listWidget->setWrapping(true);  //自动换行
    listWidget->setDragDropMode(QAbstractItemView::InternalMove);

    m_pImageView = new ImageView;
    connect(listWidget, &QListWidget::doubleClicked, [=](){
        QListWidgetItem *currListWidgetItem = listWidget->currentItem();
        if(currListWidgetItem)
        {
            if(m_iconMap[currListWidgetItem].type == 0)
            {
                m_pImageView->move(QCursor::pos() - QPoint(0, m_pImageView->height()));
                m_pImageView->show();
                m_pImageView->recvShowPic(QImage(m_iconMap[currListWidgetItem].path));
            }
            if(m_iconMap[currListWidgetItem].type == 1)
            {

                //...
            }
        }
    });
}

PropertysWgt::~PropertysWgt()
{
}

PropertyFile PropertysWgt::propertyFile(QListWidgetItem *item)
{
    return m_iconMap[item];
}

void PropertysWgt::removeAll()
{
    listWidget->clear();
    m_iconMap.clear();
}

void PropertysWgt::addIcon(QString path, int type)
{
    QFileInfo file(path);
    if(!file.isFile())
    {
        return;
    }
    QString icon = path;
    if(type != 0)
    {
        icon = ":/res/icon/modelIcon.png";
    }
    QListWidgetItem *newitem = new QListWidgetItem(          //自适应列表项中的图片大小
                QIcon(QPixmap(icon).scaled(QSize(50,50))),file.fileName());

    //设置列表项整体的尺寸
    newitem->setSizeHint(QSize(80,80));
    //设置列表项中的文字对齐方式：水平居中
    newitem->setTextAlignment(Qt::AlignHCenter);
    newitem->setToolTip(path);
    //在列表框中添加列表项
    listWidget->addItem(newitem);
    PropertyFile propertyFile;
    propertyFile.path = path;
    propertyFile.type = type;
    m_iconMap[newitem] = propertyFile;
}
