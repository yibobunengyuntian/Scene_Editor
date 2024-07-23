#include "propertyswgt.h"
#include "ui_propertyswgt.h"

PropertysWgt::PropertysWgt(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    listWidget->setViewMode(QListWidget::IconMode);//��ʾģʽ
    listWidget->setIconSize(QSize(50, 50));//����ͼƬ��С
    listWidget->setSpacing(3);//���
    listWidget->setResizeMode(QListView::Adjust);//��Ӧ���ֵ���
    listWidget->setMovement(QListView::Static);//�����ƶ�
    listWidget->setWrapping(true);  //�Զ�����
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
    QListWidgetItem *newitem = new QListWidgetItem(          //����Ӧ�б����е�ͼƬ��С
                QIcon(QPixmap(icon).scaled(QSize(50,50))),file.fileName());

    //�����б�������ĳߴ�
    newitem->setSizeHint(QSize(80,80));
    //�����б����е����ֶ��뷽ʽ��ˮƽ����
    newitem->setTextAlignment(Qt::AlignHCenter);
    newitem->setToolTip(path);
    //���б��������б���
    listWidget->addItem(newitem);
    PropertyFile propertyFile;
    propertyFile.path = path;
    propertyFile.type = type;
    m_iconMap[newitem] = propertyFile;
}
