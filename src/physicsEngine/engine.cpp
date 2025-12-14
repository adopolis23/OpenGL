#include "engine.h"


Engine::Engine(const Camera* cam)
{
    camera = cam;

    //set up the density field
    densityField.width = 200;
    densityField.height = 200;
    densityField.cellSize = cam->world_height / (float)densityField.height;
    densityField.density.resize(densityField.width * densityField.height);
}


void Engine::HandleCollisions(Object* obj)
{
    // left side of object contacts left wall
    if (obj->position.x - obj->radius < camera->left_world_bound && obj->velocity.x < 0)
    {
        obj->velocity.x *= -1;
    }

    // right side of object contacts right wall
    if (obj->position.x + obj->radius > camera->right_world_bound && obj->velocity.x > 0)
    {
        obj->velocity.x *= -1;
    }

    // top of object contacts top wall
    if (obj->position.y + obj->radius > camera->top_world_bound && obj->velocity.y > 0)
    {
        obj->velocity.y *= -1;
    }

    // bottom of object contacs bottom wall
    if (obj->position.y - obj->radius < camera->bottom_world_bound && obj->velocity.y < 0)
    {
        obj->velocity.y *= -1;
    }
}



void Engine::Update(Scene& scene, float dt)
{

    // add velocities to each particles position
    for (auto obj : scene.objects)
    {
        obj->position.x += obj->velocity.x * dt;
        obj->position.y += obj->velocity.y * dt;

        HandleCollisions(obj);
    }

    
    CalculateDensity(scene);
}

// TODO: fix this test density calculation
void Engine::CalculateDensity(const Scene& scene)
{
    float u,v;
    int gx, gy;

    // reset d-field to all 0s
    std::fill(densityField.density.begin(), densityField.density.end(), 0.0f);

    for (auto obj : scene.objects)
    {
        // convert world positions into [0,1] range
        u = (obj->position.x - camera->left_world_bound) / (camera->right_world_bound - camera->left_world_bound);
        v = (obj->position.y - camera->bottom_world_bound) / (camera->top_world_bound - camera->bottom_world_bound);

        // convert world position into bin values for the density field
        gx = int(u * densityField.width);
        gy = int(v * densityField.height);

        gx = std::clamp(gx, 0, densityField.width  - 1);
        gy = std::clamp(gy, 0, densityField.height - 1);

        
        densityField.density[(gy * densityField.width) + gx] += 1.0;

    }


}