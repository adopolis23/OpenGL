#pragma once
#include <glm/glm.hpp>
#include "shader.h"
#include <vector>

class Object
{

public:
    // the 4 dimentional model of the object
    glm::mat4 model;

    // 3d matricies for position rotation and scale
    glm::vec3 position{0.0f};
    glm::vec3 rotation{0.0f};
    glm::vec3 scale{1.0f};

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