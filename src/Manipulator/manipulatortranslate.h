#ifndef MANIPULATORTRANSLATE_H
#define MANIPULATORTRANSLATE_H

#include "manipulatorbase.h"

class ManipulatorTranslate: public ManipulatorBase
{
public:
    ManipulatorTranslate();

    bool mouseDetection(QPoint pos, int width, int height);
    void transformByMouesmove(QPoint pos, int width, int height);
    void transformStart();
    void transformEnd();

protected:
    vector<Vertex> m_verticesBase;
    QVector3D m_startPos;
};

#endif // MANIPULATORTRANSLATE_H
