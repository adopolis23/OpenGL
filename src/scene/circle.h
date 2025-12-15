#pragma once
#include "physicsObject.h"
#include "globals.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Circle : public PhysicsObject {

public:
    Circle(int sections, float radius, glm::vec2 pos);
    void GenerateObjectModel();

private:
    int SectorCount;

    void InitVelocity();

    void GenerateVerticies();

};
