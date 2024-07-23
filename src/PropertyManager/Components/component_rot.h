#ifndef COMPONENT_ROT_H
#define COMPONENT_ROT_H

#include "componentbase.h"

class Component_rot : public ComponentBase
{
public:
    Component_rot();

protected slots:
    virtual void run() override;
};

#endif // COMPONENT_ROT_H
