#include "densitySystem.h"


DensitySystem::DensitySystem(const Camera* cam, const Scene& scene)
{
    camera = cam;

    //set up the density field (not used anymore)
    densityField.width = 100;
    densityField.height = 100;
    densityField.cellSizeX = cam->world_width / (float)densityField.width;
    densityField.cellSizeY = cam->world_height / (float)densityField.height;
    densityField.density.resize(densityField.width * densityField.height);

    // ensure capacity
    particleDensity.reserve(scene.objects.size());
    particlePressureForce.reserve(scene.objects.size());
}



float DensitySystem::DensitySmoothingKernel(float radius, float dist)
{
    if (dist >= radius) return 0.0f;

    float x = 1.0f - dist / radius;
    float normalization = 10.0f / (M_PI * powf(radius, 6));

    return normalization * x * x * x;
}

float DensitySystem::DensitySmoothingKernelDerivative(float radius, float dist)
{
    if (dist >= radius) return 0.0f;
    if (dist < 1e-6f) return 0.0f;

    float x = 1.0f - dist / radius;
    float normalization = 10.0f / (M_PI * powf(radius, 6));

    return -3.0f * normalization * x * x / radius;
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
    glm::vec3 otherObjectPostion;


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

                r = position - scene.objects.at(id)->position;
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


// compute per-particle density, pressure and pairwise pressure+viscosity forces.
// Stores results in particleDensity and particlePressureForce.
// TODO: need to optimize further for performance
void DensitySystem::CalculatePressureForParticles(const Scene& scene)
{
    // compute densities
    // clear previous maps to start fresh
    particleDensity.clear();
    particlePressureForce.clear();

    // tunables to keep forces in a normal numeric range
    const float pressureScale = 0.002f;    // global scale applied to computed pressure (tune down to reduce magnitude)
    const float viscosityScale = 1.0f;    // scale for viscosity contribution; not used

	const float maxInteractionForce = 1e-7f; // clamp single neighbor contribution; these two might be redundant due to the pressure multiplyer above
    const float maxTotalForce = 1e-7f;     // clamp per-particle total force

    // updated: compute density for each particle using neighbors via spatial grid (same quad neighborhood)
    for (const auto& [id, obj] : scene.objects)
    {
        float rho = 0.0f;
        glm::vec3 pos = obj->position;

        int quadId = spatialGrid->GetQuadIdFromPosition(pos);
        int qx = quadId % spatialGrid->quadWidth;
        int qy = quadId / spatialGrid->quadWidth;

        for (int dy = -1; dy <= 1; ++dy)
        {
            for (int dx = -1; dx <= 1; ++dx)
            {
                int nx = qx + dx;
                int ny = qy + dy;
                if (nx < 0 || ny < 0 || nx >= spatialGrid->quadWidth || ny >= spatialGrid->quadHeight) continue;

                int neighborQuad = ny * spatialGrid->quadWidth + nx;
                for (int nid : spatialGrid->quadid_to_objectids.at(neighborQuad))
                {
                    const auto& other = scene.objects.at(nid);
                    glm::vec3 rvec = pos - other->position;
                    float dist = glm::length(rvec);
                    if (dist > kernelRadius) continue;

                    // include particle mass in density contribution (standard SPH)
                    if (dist < 1e-6f)
                    {
                        rho += DensitySmoothingKernel(kernelRadius, 0.0f) * other->mass;
                    }
                    else
                    {
                        rho += DensitySmoothingKernel(kernelRadius, dist) * other->mass;
                    }
                }
            }
        }

        // avoid zero density
        particleDensity[id] = (rho > 1e-8f) ? rho : 1e-8f;
    }


    // compute pressure per particle, with optional clamp of negative pressures (helps stability)
    std::unordered_map<int, float> particlePressure;
    particlePressure.reserve(particleDensity.size());
    float maxRho = 0.0f;
    for (const auto& [id, rho] : particleDensity)
    {
        maxRho = std::max(maxRho, rho);

        // original physical pressure p = k * (rho - restDensity)
        float p = stiffness * (rho - restDensity);

        // clamp tensile (negative) pressure to zero to avoid attraction that causes clumping
        if (p < 0.0f) p = 0.0f;

        // scale pressure down globally to match simulation units (tune this)
        p *= pressureScale;

        particlePressure[id] = p;
    }

    // second pass - compute pairwise pressure & viscosity forces (symmetric SPH pressure)
    for (const auto& [id, obj] : scene.objects)
    {
        glm::vec2 force{ 0.0f, 0.0f };
        glm::vec3 pos = obj->position;
        glm::vec3 vel = obj->velocity;
        float rho_i = particleDensity[id];
        float p_i = particlePressure[id];

        int quadId = spatialGrid->GetQuadIdFromPosition(pos);
        int qx = quadId % spatialGrid->quadWidth;
        int qy = quadId / spatialGrid->quadWidth;

        for (int dy = -1; dy <= 1; ++dy)
        {
            for (int dx = -1; dx <= 1; ++dx)
            {
                int nx = qx + dx;
                int ny = qy + dy;
                if (nx < 0 || ny < 0 || nx >= spatialGrid->quadWidth || ny >= spatialGrid->quadHeight) continue;

                int neighborQuad = ny * spatialGrid->quadWidth + nx;
                for (int nid : spatialGrid->quadid_to_objectids.at(neighborQuad))
                {
                    if (nid == id) continue;

                    const auto& other = scene.objects.at(nid);
                    glm::vec3 rvec = pos - other->position;
                    float dist = glm::length(rvec);
                    if (dist < 1e-6f || dist > kernelRadius) continue;

                    glm::vec2 dir = glm::vec2(rvec.x, rvec.y) / dist;
                    float gradW = DensitySmoothingKernelDerivative(kernelRadius, dist);

                    // standard symmetric SPH pressure term
                    float rho_j = particleDensity[nid];
                    float p_j = particlePressure[nid];
                    float m_j = other->mass;

                    // compute pressure contribution using p_i/rho_i^2 + p_j/rho_j^2
                    float inv_rho_i2 = 1.0f / (rho_i * rho_i);
                    float inv_rho_j2 = 1.0f / (rho_j * rho_j);
                    glm::vec2 pressTerm = -m_j * (p_i * inv_rho_i2 + p_j * inv_rho_j2) * gradW * dir;

                    // clamp single-interaction pressure contribution
                    {
                        float mag2 = pressTerm.x * pressTerm.x + pressTerm.y * pressTerm.y;
                        if (mag2 > (maxInteractionForce * maxInteractionForce))
                        {
                            float inv = 1.0f / std::sqrt(mag2);
                            pressTerm *= (maxInteractionForce * inv);
                        }
                    }

                    force += pressTerm;

                    // viscosity
                    glm::vec2 velDiff = glm::vec2(other->velocity.x - vel.x, other->velocity.y - vel.y);
                    float w = DensitySmoothingKernel(kernelRadius, dist);
                    glm::vec2 viscTerm = viscosityScale * viscosity * m_j * (velDiff / rho_j) * w;

                    {
                        float vmag2 = viscTerm.x * viscTerm.x + viscTerm.y * viscTerm.y;
                        if (vmag2 > (maxInteractionForce * maxInteractionForce))
                        {
                            float inv = 1.0f / std::sqrt(vmag2);
                            viscTerm *= (maxInteractionForce * inv);
                        }
                    }

                    force += viscTerm;
                }
            }
        }

        // clamp total per-particle force to avoid huge acceleration
        {
            float fmag2 = force.x * force.x + force.y * force.y;
            if (fmag2 > (maxTotalForce * maxTotalForce))
            {
                float inv = 1.0f / std::sqrt(fmag2);
                force *= (maxTotalForce * inv);
            }
        }

        particlePressureForce[id] = force;
    }
}