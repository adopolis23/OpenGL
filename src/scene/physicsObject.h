#pragma once
#include <glm/glm.hpp>
#include "shader.h"
#include <vector>

class PhysicsObject
{

public:
    PhysicsObject()
    {
        // set this objects unique ID
        objectId = objectCount;
        objectCount++;
    }

    ~PhysicsObject()
    {
        objectCount--;
    }

    int objectId;

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

    glm::vec3 velocity; // world units per second
    
    float radius;

    // mass in grams i guess
    float mass = 1000;

    virtual void GenerateObjectModel() = 0;
    
private:

    static int objectCount;

    // the =0 makes this pure virtual so no implementation in the base class is expected
    virtual void GenerateVerticies() = 0;

};
