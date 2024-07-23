#ifndef CONFIGFILE_H
#define CONFIGFILE_H


#include <QVariant>
#include <QSettings>
#include <QApplication>

class ConfigFile
{

public:
    ConfigFile();
    bool fileName(QString qstrfilename);
    virtual ~ConfigFile(void);
    void Set(QString,QString,QVariant);
    QVariant Get(QString,QString);
private:
    QString m_qstrFileName = "ini.cfg";
    QSettings *m_psetting;
};

#endif // CONFIGFILE_H
