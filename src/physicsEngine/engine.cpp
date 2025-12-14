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

    densityField.density[5000] += 10.0f;

}