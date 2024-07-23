#ifndef CONFIGFILE_H
#define CONFIGFILE_H


#include <QVariant>
#include <QSettings>
#include <QApplication>

#ifndef g_Config
#define g_Config ConfigFile::getConfig()
#endif

class ConfigFile
{
public:
    static ConfigFile* getConfig();
    virtual ~ConfigFile(void);
    void Set(QString,QString,QVariant);
    QVariant Get(QString,QString);
protected:
    ConfigFile(QString qstrfilename = qApp->applicationDirPath()+"/ini.cfg");
private:
    QString m_qstrFileName = "ini.cfg";
    QSettings *m_psetting;
    static ConfigFile *m_pConfig;
};

#endif // CONFIGFILE_H
