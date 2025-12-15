#include "engine.h"


Engine::Engine(const Camera* cam)
{
    camera = cam;

    //set up the density field
    densityField.width = 100;
    densityField.height = 100;
    densityField.cellSizeX = cam->world_width / (float)densityField.width;
    densityField.cellSizeY = cam->world_height / (float)densityField.height;
    densityField.density.resize(densityField.width * densityField.height);
}


void Engine::HandleCollisions(Object* obj)
{
    bool collision = false;

    // left side of object contacts left wall
    if (obj->position.x - obj->radius < camera->left_world_bound && obj->velocity.x < 0)
    {
        obj->velocity.x *= -1;
        collision = true;
    }

    // right side of object contacts right wall
    if (obj->position.x + obj->radius > camera->right_world_bound && obj->velocity.x > 0)
    {
        obj->velocity.x *= -1;
        collision = true;
    }

    // top of object contacts top wall
    if (obj->position.y + obj->radius > camera->top_world_bound && obj->velocity.y > 0)
    {
        obj->velocity.y *= -1;
        collision = true;
    }

    // bottom of object contacs bottom wall
    if (obj->position.y - obj->radius < camera->bottom_world_bound && obj->velocity.y < 0)
    {
        obj->velocity.y *= -1;
        collision = true;
    }

    // handle physics only if there was a collision
    if (collision)
    {
        obj->velocity *= collisionDampingFactor;
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
    // just a constand used to make the effect of each particle lower.
    float mass = 1.0f;

    // reset d-field to all 0s
    std::fill(densityField.density.begin(), densityField.density.end(), 0.0f);

    // this tells us how manu cells around each object to check
    int kernelCellsX = int(std::ceil(kernelRadius / densityField.cellSizeX));
    int kernelCellsY = int(std::ceil(kernelRadius / densityField.cellSizeY));

    for (auto obj : scene.objects)
    {

        //convert particle world locations to [0-1] range
        float u = (obj->position.x - camera->left_world_bound) / (camera->right_world_bound - camera->left_world_bound);
        float v = (obj->position.y - camera->bottom_world_bound) / (camera->top_world_bound - camera->bottom_world_bound);

        // grid value in the density field this particle belongs too
        int gx = int(u * densityField.width);
        int gy = int(v * densityField.height);

        // clamp values to not overflow
        gx = std::clamp(gx, 0, densityField.width  - 1);
        gy = std::clamp(gy, 0, densityField.height - 1);

        // loop over all near by cells in the influence range
        for (int y = gy - kernelCellsY; y <= gy + kernelCellsY; ++y)
        {
            for (int x = gx - kernelCellsX; x <= gx + kernelCellsX; ++x)
            {
                if (x < 0 || x >= densityField.width || y < 0 || y >= densityField.height)
                    continue;
                
                // compute the world centerpoint of those cells
                float cellCenterX = camera->left_world_bound + ((x + 0.5f) * densityField.cellSizeX);
                float cellCenterY = camera->bottom_world_bound + ((y + 0.5f) * densityField.cellSizeY);
            
                // find the distance form that cell to this object
                float dx = cellCenterX - obj->position.x;
                float dy = cellCenterY - obj->position.y;
                float dist = std::sqrt(dx * dx + dy * dy);

                float w = DensitySmoothingKernel(kernelRadius, dist);

                if (w > 0.0f)
                {
                    densityField.density[y * densityField.width + x] += w * mass;
                }
            }
        }


    }
}

float Engine::DensitySmoothingKernel(float radius, float dist)
{
    // calculate the volume of the smoothing kernel to divide out at the end
    float volume = (M_PI * pow(radius, 8)) / 4;

    //calc the value and clamp it to zero
    float value = (radius * radius) - (dist * dist);
    value = (value > 0) ? value : 0;

    return ((value * value * value) / volume);
}

float Engine::DensitySmoothingKernelDerivative(float radius, float dist)
{
    if (dist > radius) return 0.0;

    float value = (radius * radius) - (dist * dist);
    float scale = -24 / (M_PI * pow(radius, 8));
    return scale * dist * value * value;
}