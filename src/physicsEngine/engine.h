#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <unordered_map>

#include "globals.h"
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

    
    // takes a physics step for objects in a scene. scaled by the time that has passed between draws `dt`
    void Update(Scene& scene, float dt);
    
    DensityField densityField;
    
    // stores particle ids and the (most recent) density gradient at their position
    std::unordered_map<int, glm::vec2> particleDensityGradient;
    
    float kernelRadius = 0.4f;
    
private:
    
    void HandleCollisions(PhysicsObject* obj);
    float collisionDampingFactor = 0.90f;
    
    // pointer to the global camera for this scene
    const Camera* camera;
    
    // radius: radius of the influence of the kernel
    // dist: distnace from the center you want to calculate influence
    float DensitySmoothingKernel(float radius, float dist);
    float DensitySmoothingKernelDerivative(float radius, float dist);
    void CalculateDensityField(const Scene& scene);
    glm::vec2 CalculateDensityGradientAtPosition(const Scene& scene, int objectId);
    float CalculateDensityAtParticle(const Scene& scene, const glm::vec3& position);
    void CalculateDensityGradientAtParticles(const Scene& scene);

    void ApplyPressureForceToParticles(Scene& scene, float dt);

};