#ifndef MANIPULATORSCALE_H
#define MANIPULATORSCALE_H
#include "manipulatorbase.h"

class ManipulatorScale: public ManipulatorBase
{
public:
    ManipulatorScale();

    bool mouseDetection(QPoint pos, int width, int height);
    void transformByMouesmove(QPoint pos, int width, int height);
    void transformStart();
    void transformEnd();

private:
    vector<Vertex> m_verticesBase;
    QVector3D m_startSca;
};

#endif // MANIPULATORSCALE_H
