#ifndef MANIPULATORROTATE_H
#define MANIPULATORROTATE_H

#include "manipulatorbase.h"

class ManipulatorRotate: public ManipulatorBase
{
public:
    ManipulatorRotate();

    bool mouseDetection(QPoint pos, int width, int height);
    void transformByMouesmove(QPoint pos, int width, int height);
    void transformStart();
    void transformEnd();

private:
    uint m_nSelIndex;
    QVector3D m_selPos;
    QVector3D m_oldRotate;
    QMatrix4x4 m_oldMatri;

    QVector3D m_startRot;
};

#endif // MANIPULATORROTATE_H
