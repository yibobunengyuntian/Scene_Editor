#ifndef PARTICLE_H
#define PARTICLE_H

#include "node.h"

class Particle: public Node
{
public:
    Particle(Node *parent = nullptr);
};

#endif // PARTICLE_H
