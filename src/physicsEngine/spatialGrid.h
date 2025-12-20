#pragma once

#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include "renderer/camera.h"
#include <algorithm>
#include "scene.h"

class SpatialGrid
{

public:
    SpatialGrid(int objectCount, float quadSize, const Camera* cam);


    //update the quad for some object id in objectid_to_quad
    void UpdateQuadLocation(int objectId, const glm::vec3 position);

    //get the id of the quad some position belongs to.
    int GetQuadIdFromPosition(const glm::vec3 position);

    void UpdateQuadLocations(const Scene& scene);

    std::vector<std::vector<int>> quadid_to_objectids;      // [quadID] -> particle IDs
    int quadWidth;
    int quadHeight;

private:

    //index into this vector is the particle id/number the value is the hash for which quad its in
    std::vector<int> objectid_to_quadid; // [particleID] -> quadID

    //size of the actual quads in world units. Going to be 2 * kernel radius
    float quadSize;

    const Camera* camera;
};