#pragma once
#include "object.h"

#include <glad/glad.h>
//#include <GLFW/glfw3.h>

#include <vector>

class Circle : public Object {

public:
    Circle(int sections, float radius);

private:
    int SectorCount;
    float radius;

    void GenerateVerticies();

};
