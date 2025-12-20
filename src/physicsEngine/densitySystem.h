#pragma once

#include <vector>
#include <unordered_map>
#include "scene.h"
#include "globals.h"
#include "renderer/camera.h"
#include "spatialGrid.h"

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


class DensitySystem
{

public:

    DensitySystem(const Camera* cam);

    //populates the cache of density gradients at each particle position
    void CalculateDensityGradientAtParticles(const Scene& scene);

    // stores particle ids and the (most recent) density gradient at their position
    std::unordered_map<int, glm::vec2> particleDensityGradient;

    DensityField densityField;

    // pointer to the engines spatial grid object
    SpatialGrid* spatialGrid;

    // radius of the density smoothing kernel; also used to compute how many quads are on the screen
    float kernelRadius = 0.3f;

private:
    
    // radius: radius of the influence of the kernel
    // dist: distnace from the center you want to calculate influence
    float DensitySmoothingKernel(float radius, float dist);

    // returns the value of the slope density smoothing kernel at some distance from the center given some radius.
    float DensitySmoothingKernelDerivative(float radius, float dist);

    // not used anymore; Given the particles in a scene calculates their contribution to a density field. Usually with cells larger than 1x1px
    void CalculateDensityField(const Scene& scene);

    // returns the value of the gradient of the density function at some position
    glm::vec2 CalculateDensityGradientAtPosition(const Scene& scene, int objectId);

    // calculates the actual density value at some position; TODO: might change name
    float CalculateDensityAtParticle(const Scene& scene, const glm::vec3& position);
    


    // reference to the worlds camera
    const Camera* camera;




};