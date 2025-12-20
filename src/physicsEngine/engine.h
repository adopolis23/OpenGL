#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <unordered_map>

#include "globals.h"
#include "scene.h"
#include "renderer/camera.h"

#include "densitySystem.h"
#include "spatialGrid.h"



class Engine
{

public:

    // engine needs a ref to the camera to compute collisions and effects with the walls
    Engine(const Camera* cam, const Scene& scene);

    
    // takes a physics step for objects in a scene. scaled by the time that has passed between draws `dt`
    void Update(Scene& scene, float dt);
    
    DensitySystem densitySystem;
    
private:
    
    void HandleCollisions(PhysicsObject* obj);
    float collisionDampingFactor = 0.90f;
    
    // pointer to the global camera for this scene
    const Camera* camera;

    SpatialGrid spatialGrid;

    void ApplyPressureForceToParticles(Scene& scene, float dt);

};