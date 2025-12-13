#pragma once
#include "object.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

class Circle : public Object {

public:
    Circle();

private:
    int SectorCount;



    void GenerateVerticies();

};
