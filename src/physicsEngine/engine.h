#pragma once

#include "scene.h"
#include "renderer/camera.h"

class Engine
{

public:

    // engine needs a ref to the camera to compute collisions and effects with the walls
    Engine(const Camera* cam);

    // takes a physics step for objects in a scene. scaled by the time that has passed between draws `dt`
    void Update(Scene& scene, float dt);

private:

    // pointer to the global camera for this scene
    Camera* camera;

};