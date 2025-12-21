#include "engine.h"


Engine::Engine(const Camera* cam, const Scene& scene)
    :densitySystem(cam), spatialGrid(scene.objectCount, densitySystem.kernelRadius*2, cam)
{
    camera = cam;

    densitySystem.spatialGrid = &spatialGrid;

}


void Engine::HandleCollisions(PhysicsObject* obj)
{
    bool collision = false;

    // left side of object contacts left wall
    if (obj->position.x - obj->radius < camera->left_world_bound && obj->velocity.x < 0)
    {
        obj->position.x = camera->left_world_bound + obj->radius;
        obj->velocity.x *= -1;
        collision = true;
    }

    // right side of object contacts right wall
    if (obj->position.x + obj->radius > camera->right_world_bound && obj->velocity.x > 0)
    {
        obj->position.x = camera->right_world_bound - obj->radius;
        obj->velocity.x *= -1;
        collision = true;
    }

    // top of object contacts top wall
    if (obj->position.y + obj->radius > camera->top_world_bound && obj->velocity.y > 0)
    {
        obj->position.y = camera->top_world_bound - obj->radius;
        obj->velocity.y *= -1;
        collision = true;
    }

    // bottom of object contacs bottom wall
    if (obj->position.y - obj->radius < camera->bottom_world_bound && obj->velocity.y < 0)
    {
        obj->position.y = camera->bottom_world_bound + obj->radius;
        obj->velocity.y *= -1;
        collision = true;
    }

    // handle physics only if there was a collision
    if (collision)
    {
        obj->velocity *= collisionDampingFactor;
    }

}


void Engine::ApplyGravity(Scene& scene)
{
    float accel_gravity = 0.00003f;

    for (auto& [id, obj] : scene.objects)
    {
        obj->velocity.y += (-1) * accel_gravity;
    }
}



void Engine::Update(Scene& scene, float dt)
{

    spatialGrid.UpdateQuadLocations(scene);

    //ApplyGravity(scene);

    for (auto& [id, obj] : scene.objects)
    {
        // add velocities to each particles position
        obj->position.x += obj->velocity.x * dt;
        obj->position.y += obj->velocity.y * dt;

        // handle collisions for that object
        HandleCollisions(obj);

    }

    //update the values in the particleDensityGradient object to store the density gradient at each particles position
    densitySystem.CalculateDensityGradientAtParticles(scene);
    ApplyPressureForceToParticles(scene, dt);

    // update the values in the DensityField object to display the density in the background
    //CalculateDensityField(scene);

}





void Engine::ApplyPressureForceToParticles(Scene& scene, float dt)
{
    float accelerationX, accelerationY;
    float constant = 0.00001f;

    float max_velocity = 0.003f;

    //f = m * a
    // therefor the acceleration to apply to the particle a = f / m

    for (auto& [id, obj] : scene.objects)
    {
        // check if there is a density gradient entry for this particle
        if (densitySystem.particleDensityGradient.find(obj->objectId) == densitySystem.particleDensityGradient.end())
        {
            continue;
        }

        accelerationX = (-1) * densitySystem.particleDensityGradient.at(obj->objectId).x / obj->mass;
        accelerationY = (-1) * densitySystem.particleDensityGradient.at(obj->objectId).y / obj->mass;

        // add this acceleration to the object velocity
        obj->velocity.x += accelerationX * dt * constant;
        obj->velocity.y += accelerationY * dt * constant;
        
        if (obj->velocity.x > max_velocity)
        {
            obj->velocity.x = max_velocity;
        }

        if (obj->velocity.x < (-1) * max_velocity)
        {
            obj->velocity.x = (-1) * max_velocity;
        }

        if (obj->velocity.y > max_velocity)
        {
            obj->velocity.y = max_velocity;
        }

        if (obj->velocity.y < (-1) * max_velocity)
        {
            obj->velocity.y = (-1) * max_velocity;
        }
    }
}