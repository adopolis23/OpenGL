#pragma once

#include <vector>
#include <unordered_map>
#include "physicsObject.h"
#include "../renderer/camera.h"

class Scene
{
    
public:
    //std::vector<PhysicsObject*> objects;
    Scene(const Camera& cam);

    std::unordered_map<int, PhysicsObject*> objects;
    void AddObjectToScene(PhysicsObject* object);

private:

    const Camera* camera;

    //index into this vector is the particle id/number the value is the hash for which quad its in
    std::vector<int> objectQuads;

    int quadWidth;
    int quadHeight;

};