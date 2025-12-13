#pragma once
#include "object.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Circle : public Object {

public:
    Circle(int sections, float radius, glm::vec2 pos);
    void GenerateObjectModel();

private:
    int SectorCount;
    float radius;

    void GenerateVerticies();

};
