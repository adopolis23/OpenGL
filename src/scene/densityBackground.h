#pragma once
#include "object.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

class DensityBackground : public Object
{

public:
    DensityBackground();
    void GenerateObjectModel();

private:
    void GenerateVerticies();

};