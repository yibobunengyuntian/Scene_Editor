#include "lightwgt.h"
#include "ui_lightwgt.h"

LightWgt::LightWgt(QToolButton *bindBtn, QWidget *parent) :
    QWidget(parent), m_pBindBtn(bindBtn)
{
    setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    resBtn->setStyleSheet(m_styleSheet);
    resBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    resBtn->setIcon(QIcon(":/res/icon/res.png"));
    //resBtn->setToolTip(QStringLiteral("опо├"));

    dirSpinBox_x->setValue(static_cast<double>(m_lightDir.x()));
    dirSpinBox_y->setValue(static_cast<double>(m_lightDir.y()));
    dirSpinBox_z->setValue(static_cast<double>(m_lightDir.z()));

    dirSlider_x->setValue(static_cast<int>((m_lightDir.x() + 1)/2.0f *99.0f));
    dirSlider_y->setValue(static_cast<int>((m_lightDir.y() + 1)/2.0f *99.0f));
    dirSlider_z->setValue(static_cast<int>((m_lightDir.z() + 1)/2.0f *99.0f));

    QDoubleSpinBox *SpinBox = dirSpinBox_x;
    connect(dirSlider_x, &QSlider::valueChanged, this, [SpinBox](int value){
        SpinBox->setValue(value/100.0-1);
    });
    SpinBox = dirSpinBox_y;
    connect(dirSlider_y, &QSlider::valueChanged, this, [SpinBox](int value){
        SpinBox->setValue(value/100.0-1);
    });
    SpinBox = dirSpinBox_z;
    connect(dirSlider_z, &QSlider::valueChanged, this, [SpinBox](int value){
        SpinBox->setValue(value/100.0-1);
    });
    connect(dirSpinBox_x, SIGNAL(valueChanged(double)),this,SLOT(dirCheckBox_x_changed(double)));
    connect(dirSpinBox_y, SIGNAL(valueChanged(double)),this,SLOT(dirCheckBox_y_changed(double)));
    connect(dirSpinBox_z, SIGNAL(valueChanged(double)),this,SLOT(dirCheckBox_z_changed(double)));
    connect(camDirCheckBox, SIGNAL(clicked(bool)),SLOT(camDirCheckBoxChanged(bool)));
}

LightWgt::~LightWgt()
{
    if(m_pBindBtn)
    {
        delete []m_pBindBtn;
        m_pBindBtn = nullptr;
    }
}

void LightWgt::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event)
    if(m_pBindBtn)
    {
        m_pBindBtn->setDown(false);
    }
}

void LightWgt::dirCheckBox_x_changed(double x)
{
    m_lightDir.setX(static_cast<float>(x));
    dirSlider_x->setValue(static_cast<int>((m_lightDir.x() + 1)*100));
    emit lightDirChanged(m_lightDir);
}
void LightWgt::dirCheckBox_y_changed(double y)
{
    m_lightDir.setY(static_cast<float>(y));
    dirSlider_y->setValue(static_cast<int>((m_lightDir.y() + 1)*100));
    emit lightDirChanged(m_lightDir);
}
void LightWgt::dirCheckBox_z_changed(double z)
{
    m_lightDir.setZ(static_cast<float>(z));
    dirSlider_z->setValue(static_cast<int>((m_lightDir.z() + 1)*100));
    emit lightDirChanged(m_lightDir);
}

void LightWgt::camDirCheckBoxChanged(bool is)
{
    emit isUseCameraDir(is);
}

