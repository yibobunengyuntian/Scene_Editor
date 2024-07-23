#ifndef TITEBAR_H
#define TITEBAR_H

#include <QWidget>
#include <QMouseEvent>
#include <QPoint>
#include <QToolButton>
#include <QLabel>
#include "ui_titebar.h"

class NewMainWgt;
class TiteBar : public QWidget, public Ui_TiteBar
{
    Q_OBJECT

public:
    explicit TiteBar(QWidget *parent = nullptr);
    ~TiteBar();

    void setMaxIcon(bool arg);
    void setWindowTitle(QString _title);
    void setIcon(QString icon);
    void hideMid_maxAndMin();

signals:
    void quitEvent();
    void mid_maxEvent();
    void minEvent();
    void aboutEvent();
    void configEvent();
    void personEvent();
    void moveWindow(QPoint& pos);



protected slots:
    void onCloseWgt();
    void onMaxWgt();
    void onMinWgt();
//    void on_pb_about_clicked();
//    void on_pb_config_clicked();
//    void on_pb_person_clicked();

protected:
    void initialize();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void mouseDoubleClickEvent(QMouseEvent *event);

private:
    bool m_bPressed = false;
    QPoint pLast;
    QVBoxLayout *m_pVBox     = nullptr;
    QLabel *m_pTiteIcon      = nullptr;
    QLabel *m_pTiteText      = nullptr;
    QToolButton *minimizeBtn = nullptr;
    QToolButton *maximizeBtn = nullptr;
    QToolButton *closeBtn    = nullptr;
    QString m_styleSheet = QString("") + "QToolButton {\
                background-color: transparent;\
                color: rgb(255, 255, 255);\
                text-align: center;\
                border: none;\
                font-size: 12px;\
                min-height: 25px;\
                max-height: 25px;\
                min-width: 25px;\
                max-width: 25px;\
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

#endif // TITEBAR_H
