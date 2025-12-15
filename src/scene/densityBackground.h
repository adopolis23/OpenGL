#pragma once
#include "physicsObject.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

class DensityBackground : public PhysicsObject
{

public:
    DensityBackground();
    void GenerateObjectModel();

private:
    void GenerateVerticies();

};