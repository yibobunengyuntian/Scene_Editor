#ifndef PROPERTYSWGT_H
#define PROPERTYSWGT_H

#include <QWidget>
#include "ui_propertyswgt.h"
#include <QListWidgetItem>
#include <QDir>
#include <QDebug>
#include <QStandardItemModel>
#include "ToolWgts/ImageView/imageview.h"

#ifndef MODEL_FILE_TYPE
#define MODEL_FILE_TYPE "*.obj *.fbx *.3ds"
#endif

struct PropertyFile
{
    QString path = "";
    int type = -1;
};

class PropertysWgt : public QWidget, public Ui_PropertysWgt
{
    Q_OBJECT

public:
    explicit PropertysWgt(QWidget *parent = nullptr);
    ~PropertysWgt();

    PropertyFile propertyFile(QListWidgetItem *item);
    void removeAll();

public slots:
    void addIcon(QString path, int type);

private:
    QMap<QListWidgetItem*,PropertyFile> m_iconMap;

    ImageView *m_pImageView = nullptr;
};

#endif // PROPERTYSWGT_H
