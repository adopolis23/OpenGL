#include "densitySystem.h"


DensitySystem::DensitySystem(const Camera* cam)
{
    camera = cam;

    //set up the density field (not used anymore)
    densityField.width = 100;
    densityField.height = 100;
    densityField.cellSizeX = cam->world_width / (float)densityField.width;
    densityField.cellSizeY = cam->world_height / (float)densityField.height;
    densityField.density.resize(densityField.width * densityField.height);
}



float DensitySystem::DensitySmoothingKernel(float radius, float dist)
{
    // calculate the volume of the smoothing kernel to divide out at the end
    float volume = (M_PI * pow(radius, 8)) / 4;

    //calc the value and clamp it to zero
    float value = (radius * radius) - (dist * dist);
    value = (value > 0) ? value : 0;

    return ((value * value * value) / volume);
}

float DensitySystem::DensitySmoothingKernelDerivative(float radius, float dist)
{
    if (dist > radius) return 0.0;

    float value = (radius * radius) - (dist * dist);
    float scale = -24 / (M_PI * pow(radius, 8));
    return scale * dist * value * value;
}


// not used anymore
void DensitySystem::CalculateDensityField(const Scene& scene)
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
        gx = std::clamp(gx, 0, densityField.width - 1);
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



glm::vec2 DensitySystem::CalculateDensityGradientAtPosition(const Scene& scene, int objectId)
{
    glm::vec2 densityGradient = { 0.0f, 0.0f };
    glm::vec2 r, direction;
    float slope, dist;

    glm::vec3 position = scene.objects.at(objectId)->position;


    // get the quad id of this object
    int quadId = spatialGrid->GetQuadIdFromPosition(position);

    int quadId_x = quadId % spatialGrid->quadWidth;
    int quadId_y = quadId / spatialGrid->quadWidth;


    for (int dy = -1; dy <= 1; ++dy)
    {
        for (int dx = -1; dx <= 1; ++dx)
        {
            //if (dx == 0 && dy == 0) continue;

            int nx = quadId_x + dx;
            int ny = quadId_y + dy;

            if (nx < 0 || ny < 0 || nx >= spatialGrid->quadWidth || ny >= spatialGrid->quadHeight)
                continue;

            int neighborQuad = ny * spatialGrid->quadWidth + nx;

            for (int id : spatialGrid->quadid_to_objectids.at(neighborQuad))
            {
                if (id == objectId) continue;

                r = glm::vec2(position.x, position.y) - glm::vec2(scene.objects.at(id)->position.x, scene.objects.at(id)->position.y);
                dist = glm::length(r);

                if (dist < 1e-6f) continue; // check for extreamly small distances to prevent nan corruption apparently?

                direction = r / dist;

                slope = DensitySmoothingKernelDerivative(kernelRadius, dist);

                densityGradient += (1) * slope * direction;
            }
        }
    }

    return densityGradient;
}


float DensitySystem::CalculateDensityAtParticle(const Scene& scene, const glm::vec3& position)
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



void DensitySystem::CalculateDensityGradientAtParticles(const Scene& scene)
{
    glm::vec2 densityGradient;

    for (const auto& [id, obj] : scene.objects)
    {
        densityGradient = CalculateDensityGradientAtPosition(scene, obj->objectId);
        particleDensityGradient[obj->objectId] = densityGradient;
    }

}