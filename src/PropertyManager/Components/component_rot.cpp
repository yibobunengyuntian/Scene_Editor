#include "component_rot.h"

Component_rot::Component_rot()
{
    m_para.insert("rotation axis", QVector3D(0, 1, 0));
    m_para.insert("angle", 100);
}

void Component_rot::run()
{
    if(m_pBindNode)
    {
        QVector3D rot = m_para["rotation axis"].value<QVector3D>();
        rot /= rot.length();
        m_pBindNode->rotate(rot * m_para["angle"].toDouble()/g_PropertyManager->fps());
        //...
    }
}
