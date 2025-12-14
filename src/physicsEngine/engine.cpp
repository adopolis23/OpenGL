#include "engine.h"


Engine::Engine(const Camera* cam)
{
    camera = cam;

    //set up the density field
    densityField.width = 100;
    densityField.height = 100;
    densityField.cellSize = cam->world_height / (float)densityField.height;
    densityField.density.resize(densityField.width * densityField.height);
}

void Engine::Update(Scene& scene, float dt)
{

    for (auto obj : scene.objects)
    {
        continue; 
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


    //densityField.density[0 * 0 + 0] += 10.0;


}