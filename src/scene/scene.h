#pragma once

#include <vector>
#include <unordered_map>
#include "physicsObject.h"

class Scene
{
    
public:
    std::unordered_map<int, PhysicsObject*> objects;
    void AddObjectToScene(PhysicsObject* object);

    int objectCount = 0;

private:

};