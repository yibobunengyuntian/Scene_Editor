#include "newprojectwgt.h"

NewProjectWgt::NewProjectWgt(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);//无边框
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

// 退出按钮处理事件
void NewProjectWgt::quitEvent()
{
    close();
}
// 最大化最小化窗口按钮事件
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
// 最小化窗口事件
void NewProjectWgt::minEvent()
{

     showMinimized();
}
// 关于按钮处理事件
void NewProjectWgt::aboutEvent()
{

}
// 配置事件
void NewProjectWgt::configEvent()
{

}
// 个人信息事件
void NewProjectWgt::personEvent()
{

}

// 拖动窗口标题，移动窗口
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
            m_pLabTips->setText("<font color=red>保存路径不存在，请选择已有路径或手动创建后再选择</font>");
            m_pLEdtDirectory->clear();
            return;
        }
        close();
        return;
    }

    m_pLabTips->setText("<font color=red>项目名称和保存路径不能为空</font>");

}

void NewProjectWgt::onBrowse()
{
    QString dir = "./../data/worlds";
    if(dir.isEmpty()){
        dir = "c:/";
    }

    QString projectDir = QFileDialog::getExistingDirectory(nullptr,("项目路径"),dir);
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
//        m_pLabTips->setText("<font color=red>"+ ("项目已存在") + "</font>");
//    }else{
        m_pLabTips->clear();
//    }
}

void NewProjectWgt::initialize()
{
    TiteBar *tite = new TiteBar(this);
    tite->hideMid_maxAndMin();
    tite->setWindowTitle(("新建项目"));
    this->setWindowTitle(("新建项目"));
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
