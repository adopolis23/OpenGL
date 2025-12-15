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
    float cellSizeX; // how many world units per cell
    float cellSizeY;

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
    float collisionDampingFactor = 0.95f;

    // pointer to the global camera for this scene
    const Camera* camera;

    // todo: might want to move all the density stuff to seperate file sometime
    // radius: radius of the influence of the kernel
    // dist: distnace from the center you want to calculate influence
    float DensitySmoothingKernel(float radius, float dist);
    float DensitySmoothingKernelDerivative(float radius, float dist);
    float kernelRadius = 0.4f;

};