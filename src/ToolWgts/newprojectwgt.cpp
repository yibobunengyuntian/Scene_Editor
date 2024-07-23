#include "newprojectwgt.h"

NewProjectWgt::NewProjectWgt(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);//�ޱ߿�
    setWindowModality(Qt::ApplicationModal);
    setAttribute(Qt::WA_DeleteOnClose);

    initialize();
}

NewProjectWgt::~NewProjectWgt()
{

}

QString NewProjectWgt::getDirectory()
{
    return m_pLEdtDirectory->text().trimmed() + "/" + m_pLEdtName->text().trimmed();
}

// �˳���ť�����¼�
void NewProjectWgt::quitEvent()
{
    close();
}
// �����С�����ڰ�ť�¼�
void NewProjectWgt::mid_maxEvent()
{
    if(this->isMaximized())
    {
          showNormal();
    }
    else {
        showMaximized();
    }
}
// ��С�������¼�
void NewProjectWgt::minEvent()
{

     showMinimized();
}
// ���ڰ�ť�����¼�
void NewProjectWgt::aboutEvent()
{

}
// �����¼�
void NewProjectWgt::configEvent()
{

}
// ������Ϣ�¼�
void NewProjectWgt::personEvent()
{

}

// �϶����ڱ��⣬�ƶ�����
void NewProjectWgt::moveWindowEvent(QPoint& pos)
{
    if(this->isMaximized()) return;
    QPoint tmp = this->pos() + pos;
    this->move(tmp);
}

void NewProjectWgt::onOK()
{
    if(
            !m_pLEdtDirectory->text().isEmpty() &&
            !m_pLEdtName->text().isEmpty()){
        QString directory = m_pLEdtDirectory->text().trimmed();
        QDir dir(directory);
        if(!dir.exists()){
            m_pLabTips->setText("<font color=red>"+
                                QStringLiteral("����·�������ڣ���ѡ������·�����ֶ���������ѡ��") + "</font>");
            m_pLEdtDirectory->clear();
            return;
        }
        close();
        return;
    }

    m_pLabTips->setText("<font color=red>"+
                        QStringLiteral("��Ŀ���ƺͱ���·������Ϊ��") + "</font>");

}

void NewProjectWgt::onBrowse()
{
    QString dir = "./../data/worlds";
    if(dir.isEmpty()){
        dir = "c:/";
    }

    QString projectDir = QFileDialog::getExistingDirectory(nullptr,QStringLiteral("��Ŀ·��"),dir);
    if(projectDir.isEmpty()){
        return;
    }

    m_pLEdtDirectory->setText(projectDir);
}

void NewProjectWgt::onNameChanged(QString name)
{
    QString directory = m_pLEdtDirectory->text().trimmed();

    directory += "/" + name;

//    if(ALOF_StaticFun::isDirectoryExsit(directory)){
//        m_pLabTips->setText("<font color=red>"+ QStringLiteral("��Ŀ�Ѵ���") + "</font>");
//    }else{
        m_pLabTips->clear();
//    }
}

void NewProjectWgt::initialize()
{
    TiteBar *tite = new TiteBar(this);
    tite->hideMid_maxAndMin();
    tite->setWindowTitle(QStringLiteral("�½���Ŀ"));
    this->setWindowTitle(QStringLiteral("�½���Ŀ"));
    lay_tite->addWidget(tite);
    connect(tite,SIGNAL(quitEvent()),this,SLOT(quitEvent()));
    connect(tite,SIGNAL(mid_maxEvent()),this,SLOT(mid_maxEvent()));
    connect(tite,SIGNAL(minEvent()),this,SLOT(minEvent()));
    connect(tite,SIGNAL(aboutEvent()),this,SLOT(aboutEvent()));
    connect(tite,SIGNAL(configEvent()),this,SLOT(configEvent()));
    connect(tite,SIGNAL(personEvent()),this,SLOT(personEvent()));
    connect(tite,SIGNAL(moveWindow(QPoint&)),this,SLOT(moveWindowEvent(QPoint&)));

    QString dir = "./../data/worlds";
    if(dir.isEmpty()){
        dir = "c:/";
    }
    m_pLEdtDirectory->setText(dir);

    connect(m_pBtnCancel,SIGNAL(clicked(bool)),this,SLOT(quitEvent()));
    connect(m_pBtnOK,SIGNAL(clicked(bool)),this,SLOT(onOK()));
    connect(m_pBtnBrowse,SIGNAL(clicked(bool)),this,SLOT(onBrowse()));

    connect(m_pLEdtName,SIGNAL(textChanged(QString)),this,SLOT(onNameChanged(QString)));
}
