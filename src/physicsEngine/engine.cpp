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
    // reset d-field to all 0s
    std::fill(densityField.density.begin(), densityField.density.end(), 0.0f);

    for (auto obj : scene.objects)
    {
        int x_bin = (obj->position.x * (densityField.width / 2)) + (densityField.width / 2);
        
        for (int y = 0; y < 100; y++)
        {
            densityField.density[(y*100) + x_bin] += 1.0;
        }
    }



}