#include "spatialGrid.h"

SpatialGrid::SpatialGrid(int objectCount, float quadSize, const Camera* cam)
    :camera(cam), quadSize(quadSize)
{

    quadWidth = static_cast<int>(std::ceil(camera->world_width / quadSize));
    quadHeight = static_cast<int>(std::ceil(camera->world_height / quadSize));

    objectid_to_quadid.resize(objectCount + 1);
    quadid_to_objectids.resize(quadWidth * quadHeight);


    // this might be overkill but to avoid resizing during execution reserve space for all particles in each quad up front
    for (auto& quad : quadid_to_objectids)
    {
        quad.reserve(objectCount);
    }
}


int SpatialGrid::GetQuadIdFromPosition(const glm::vec3 position)
{
    int x = static_cast<int>((position.x - camera->left_world_bound) / quadSize);
    int y = static_cast<int>((position.y - camera->bottom_world_bound) / quadSize);

    x = std::clamp(x, 0, quadWidth - 1);
    y = std::clamp(y, 0, quadHeight - 1);

    return y * quadWidth + x;
}



void SpatialGrid::UpdateQuadLocation(int objectId, const glm::vec3 position)
{
    int quad_id = GetQuadIdFromPosition(position);

    //update quad for this particles id
    objectid_to_quadid[objectId] = quad_id;

    // this structure must be cleared before objects start getting pushed for this render call
    quadid_to_objectids.at(quad_id).push_back(objectId);

}

void SpatialGrid::UpdateQuadLocations(const Scene& scene)
{
    // clear the current record of which quads each particle is in
    for (auto& q : quadid_to_objectids)
        q.clear();

    for (auto& [id, obj] : scene.objects)
    {
        UpdateQuadLocation(id, obj->position);
    }
}