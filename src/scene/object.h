#pragma once
#include <glm/glm.hpp>
#include "shader.h"
#include <vector>

class Object
{

public:
    glm::mat4 model;

    // vertex array object
    unsigned int vao = 0;
    
    unsigned int vbo = 0;

    int VertexCount = 0;
    std::vector<float> Verticies;

    // going to swap this out for a material later maybe?
    Shader* shader;
    //Material* material = nullptr;

    virtual ~Object() {}

private:

    //virtual void GenerateVerticies();

};