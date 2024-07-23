#ifndef MAINWGT_H
#define MAINWGT_H

#include <QWidget>
#include "ui_mainwgt.h"
#include "plc.h"
#include "renderwgt.h"

const static QString g_CHECKTOOLSTYLE = "QToolButton{"
                                        "spacing: 3px;"
                                        //"background-color: rgb(51, 51, 51);"
                                        "color: white;"
                                        "min-height: 20px;"
                                        "max-height: 20px;"
                                        "border: none;"
                                        "}"
                                        "QToolButton:hover {"
                                        "    background-color: rgb(42, 195, 255);"
                                        "border: none;"
                                        "}"
                                        "QToolButton:checked {"
                                        "    background-color: rgb(32, 175, 235);"
                                        "}";

class MainWgt : public QWidget, public Ui_MainWgt
{
    Q_OBJECT

    enum LabelBorderStatus {
           unKnown = 0,
           toTop = 1,
           toBottom = 2,
           toLeft = 3,
           toRight = 4,
           toLeftTop = 5,
           toRightTop = 6,
           toLeftBottom = 7,
           toRightBottom = 8
        };

public:
    explicit MainWgt(QWidget *parent = nullptr);
    ~MainWgt();

public slots:
    void setTaskbarProgressValue(int value);
private slots:
    void quitEvent();
    void mid_maxEvent();
    void minEvent();
    void aboutEvent();
    void configEvent();
    void personEvent();
    void moveWindowEvent(QPoint& pos);
    void onModeChanged(bool is);
//    void onWidgetSelectBtnClicked();
    void onLightWgtBtnClicked();
    void onNew();
    void onOPen();
    void onSave();

protected:
    void initWidget();
//    void paintEvent(QPaintEvent* event);
    bool eventFilter(QObject *target, QEvent *event);
    MainWgt::LabelBorderStatus testLabelBorder(QString _objectName);
    void moveAndResizeWindow(QPoint& pos);

    QVBoxLayout *m_pVBox = nullptr;
    TiteBar *m_pTiteBar = nullptr;
    QToolBar *pToolBar = nullptr;
    RenderWgt *m_pRenderWgt = nullptr;
//    QToolButton *m_pWidgetSelectBtn = nullptr;
    QToolButton *m_pLightWgtBtn = nullptr;
//    CheckBoxMenu *m_pWidgetSelectMenu = nullptr;
    LightWgt *m_pLightWgt = nullptr;
    LabelBorderStatus mWp;
    QMouseEvent* mouseEvent = nullptr;
    bool m_bPressed;
    QPoint pLast;

    QWinTaskbarButton *m_pWindowsTaskbarButton = nullptr;
    QWinTaskbarProgress *m_pWindowsTaskbarProgress = nullptr;

    QString m_styleSheet = QString("") + "QToolButton {\
                background-color: transparent;\
                color: rgb(255, 255, 255);\
                text-align: center;\
                border: none;\
                font-size: 12px;\
                min-height: 20px;\
                max-height: 20px;\
                min-width: 20px;\
                max-width: 20px;\
                \
            }\
            \
            QToolButton:hover {\
                background-color: rgb(200, 0, 0);\
                color: rgb(255, 255, 255);\
                text-align: center;\
                \
            }\
            \
            QToolButton:checked,\
                QToolButton:pressed {\
                background-color: rgb(255, 0, 0);\
                color: rgb(255, 255, 255);\
                text-align: center;\
            }";
};

#endif // MAINWGT_H
