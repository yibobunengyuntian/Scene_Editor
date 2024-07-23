#include "configFile.h"
#include <QDebug>
#include <QDir>
ConfigFile::ConfigFile()
{

}

bool ConfigFile::fileName(QString qstrfilename)
{
    if (qstrfilename.isEmpty())
    {
        m_qstrFileName = qApp->applicationDirPath()+"/ini.cfg";
    }
    else
    {
        m_qstrFileName = qstrfilename;
    }
    QFile file(m_qstrFileName);
    file.open(QIODevice::ReadWrite | QIODevice::Append);
    file.close();
    m_psetting = new QSettings(m_qstrFileName, QSettings::IniFormat);
    return true;
}

ConfigFile::~ConfigFile()
{
    delete m_psetting;
    m_psetting = 0;
}

void ConfigFile::Set(QString qstrNodeName,QString qstrKeyName,QVariant qvarvalue)
{
    m_psetting->setValue(QString("/%1/%2").arg(qstrNodeName).arg(qstrKeyName), qvarvalue);
}

QVariant ConfigFile::Get(QString qstrNodeName,QString qstrKeyName)
{
    QVariant qvar = m_psetting->value(QString("/%1/%2").arg(qstrNodeName).arg(qstrKeyName));
    return qvar;
}

