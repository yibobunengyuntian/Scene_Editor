#ifndef NODEEDITWGT_H
#define NODEEDITWGT_H

#include "ui_nodeeditwgt.h"
#include "Nodes/node.h"

class NodeEditWgt : public QWidget, public Ui_NodeEditWgt
{
    Q_OBJECT

public:
    explicit NodeEditWgt(QWidget *parent = nullptr);

    void update();
public slots:
    void bindNode(Node *node);

protected slots:
    void onMaterialChanged();
    void onNameEditChanaged(const QString &text);
    void onTraChanged();
    void onRotChanged();
    void onScaChanged();
    void onTextureChanged();

private:
    Node *m_pBindNode = nullptr;
    bool m_isUpdate = false;

    QString m_styleSheet = QString("") + "QToolButton {\
                background-color: rgb(118, 118, 118);\
                color: rgb(255, 255, 255);\
                text-align: center;\
                border: none;\
                font-size: 12px;\
                min-height: 15px;\
                max-height: 15px;\
                min-width: 15px;\
                max-width: 15px;\
                \
            }\
            \
            QToolButton:hover {\
                background-color: rgb(99, 99, 99);\
                color: rgb(255, 255, 255);\
                text-align: center;\
                \
            }\
            \
            QToolButton:checked,\
                QToolButton:pressed {\
                background-color: rgb(55, 55, 55);\
                color: rgb(255, 255, 255);\
                text-align: center;\
            }";

};

#endif // NODEEDITWGT_H
