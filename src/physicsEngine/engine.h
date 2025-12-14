#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

#include "scene.h"
#include "renderer/camera.h"


// struct to contain information about the density at each point in space with
// a variable resolution
struct DensityField
{

    int width;
    int height;
    float cellSize; // how many world units per cell

    std::vector<float> density;

};




class Engine
{

public:

    // engine needs a ref to the camera to compute collisions and effects with the walls
    Engine(const Camera* cam);

    void CalculateDensity(const Scene& scene);

    // takes a physics step for objects in a scene. scaled by the time that has passed between draws `dt`
    void Update(Scene& scene, float dt);

    DensityField densityField;
private:

    void HandleCollisions(Object* obj);

    // pointer to the global camera for this scene
    const Camera* camera;

};