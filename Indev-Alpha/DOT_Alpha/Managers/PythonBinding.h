#ifndef PYTHONBINDING_H
#define PYTHONBINDING_H
#include "World.h"

extern "C"
{
    World* GetWorld() { return World::get(); }
    void resetWorld() { World::get()->resetWorld(); }
    void simulateStep() { World::get()->simulationStep(); }
    void simulateGroup_CPU(IEntityGroup* Group) { World::get()->simulateGroup_CPU(*Group); }


}
#endif // PYTHONBINDING_H
