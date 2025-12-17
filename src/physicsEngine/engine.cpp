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



void Engine::Update(Scene& scene, float dt)
{

    for (auto& [id, obj] : scene.objects)
    {
        // add velocities to each particles position
        obj->position.x += obj->velocity.x * dt;
        obj->position.y += obj->velocity.y * dt;

        // handle collisions for that object
        HandleCollisions(obj);
    }

    //update the values in the particleDensityGradient object to store the density gradient at each particles position
    CalculateDensityGradientAtParticles(scene);
    ApplyPressureForceToParticles(scene, dt);

    // update the values in the DensityField object to display the density in the background
    CalculateDensityField(scene);

}


void Engine::CalculateDensityField(const Scene& scene)
{
    // just a constand used to make the effect of each particle lower.
    float mass = 1.0f;

    // reset d-field to all 0s
    std::fill(densityField.density.begin(), densityField.density.end(), 0.0f);

    // this tells us how manu cells around each object to check
    int kernelCellsX = int(std::ceil(kernelRadius / densityField.cellSizeX));
    int kernelCellsY = int(std::ceil(kernelRadius / densityField.cellSizeY));

    for (const auto& [id, obj] : scene.objects)
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

                if (dist > kernelRadius) continue;

                float w = DensitySmoothingKernel(kernelRadius, dist);

                if (w > 0.0f)
                {
                    densityField.density[y * densityField.width + x] += w * mass;
                }
            }
        }


    }
}

glm::vec2 Engine::CalculateDensityGradientPosition(const Scene& scene, int objectId)
{
    glm::vec2 densityGradient = {0.0f, 0.0f};
    glm::vec2 r, direction;
    float slope;

    glm::vec3 position = scene.objects.at(objectId)->position; 

    for (const auto& [id, obj] : scene.objects)
    {
        if (id == objectId) continue;

        r = glm::vec2(position.x, position.y) - glm::vec2(obj->position.x, obj->position.y);

        float dist = glm::length(r);

        if (dist <= 0.0f || dist > kernelRadius) continue;

        direction = r / dist;

        slope = DensitySmoothingKernelDerivative(kernelRadius, dist);

        densityGradient += (1) * slope * direction;
    }

    return densityGradient;
}

float Engine::CalculateDensityAtParticle(const Scene& scene, const glm::vec3& position)
{
    float densityValue = 0.0f;
    float dx, dy, dist;

    for (const auto& [id, obj] : scene.objects)
    {
        dx = position.x - obj->position.x;
        dy = position.y - obj->position.y;
        dist = std::sqrt(dx * dx + dy * dy);

        if (dist > kernelRadius) continue;

        densityValue += DensitySmoothingKernel(kernelRadius, dist);
    }

    return densityValue;
}

void Engine::CalculateDensityGradientAtParticles(const Scene& scene)
{
    glm::vec2 densityGradient;

    for (const auto& [id, obj] : scene.objects)
    {
        densityGradient = CalculateDensityGradientPosition(scene, obj->objectId);
        particleDensityGradient[obj->objectId] = densityGradient;
    }

}


void Engine::ApplyPressureForceToParticles(Scene& scene, float dt)
{
    float accelerationX, accelerationY;
    float constant = 0.00001f;

    //f = m * a
    // therefor the acceleration to apply to the particle a = f / m

    for (auto& [id, obj] : scene.objects)
    {
        // check if there is a density gradient entry for this particle
        if (particleDensityGradient.find(obj->objectId) == particleDensityGradient.end())
        {
            continue;
        }

        accelerationX = (-1) * particleDensityGradient.at(obj->objectId).x / obj->mass;
        accelerationY = (-1) * particleDensityGradient.at(obj->objectId).y / obj->mass;

        // add this acceleration to the object velocity
        obj->velocity.x += accelerationX * dt * constant;
        obj->velocity.y += accelerationY * dt * constant;
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