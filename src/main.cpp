#include <QApplication>
#include <windows.h>
#include <QMutex>
#include<QDateTime>

#include "plc.h"

void logout(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // 加锁
    static QMutex mutex;
    mutex.lock();

    static bool isEmptyFile = false;
    if(!isEmptyFile)
    {
        isEmptyFile = true;
        QFile file(qApp->applicationDirPath() +"/log.txt");
        file.open(QFile::WriteOnly|QFile::Truncate);
        file.close();
    }
    QString strMsg("");
    switch(type)
    {
    case QtDebugMsg:
        strMsg = QString("Debug:");
        break;
    case QtWarningMsg:
        strMsg = QString("Warning:");
        break;
    case QtCriticalMsg:
        strMsg = QString("Critical:");
        break;
    case QtFatalMsg:
        strMsg = QString("Fatal:");
        break;
    }

    // 设置输出信息格式
    QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    QString strMessage = QString("Message:%1 %2 File:%3  Line:%4  Function:%5  DateTime:%6")
            .arg(strMsg).arg(msg).arg(context.file).arg(context.line).arg(context.function).arg(strDateTime);

    // 输出信息至文件中（读写、追加形式）
    QFile file(qApp->applicationDirPath() +"/log.txt");
    file.open(QIODevice::ReadWrite | QIODevice::Append);
    QTextStream stream(&file);
    stream << strMessage << "\r\n";
    file.flush();
    file.close();

    // 解锁
    mutex.unlock();
}

int main(int argc, char *argv[])
{
//    qInstallMessageHandler(logout);   //将调试信息输出到文件
    QGuiApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
    QApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
    QApplication a(argc, argv);
    QFont f("Tahoma");
    qApp->setFont(f);
//    font: 12px "Tahoma";
//    QPixmap pixmap(":/res/icon/loading.png");
//    QSplashScreen splash(pixmap);
//    splash.show();

    StartWgt w;
    w.show();
//    splash.finish(&w);

    return a.exec();

//    QSplashScreen* splash = new QSplashScreen();

//        QLabel label(reinterpret_cast<QWidget*>(splash));
//        QMovie mv(":/res/gif/loading.gif");
//        label.setMovie(&mv);
//        mv.start();
//        splash->show();
//        splash->setCursor(Qt::BlankCursor);
//        for (int i = 0; i < 3000; i += mv.speed()) {
//            a.processEvents(); //防止界面出现无响应
////            qDebug() << QString::number(mv.speed());
//            QThread::msleep(mv.speed());
//        }
//        MainWgt w;

//        w.show();
//        splash->finish(&w);
//        return a.exec();
}
