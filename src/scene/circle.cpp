#include "circle.h"
#include <iostream>
#include "globals.h"

void Circle::GenerateVerticies()
{

    // how big of a slice each of the triangles will take up
    float thetaStep = 2.0f * M_PI / SectorCount;

    for (int sector = 0; sector < SectorCount; sector++)
    {
        float a0 = thetaStep * sector;
        float a1 = thetaStep * (sector + 1);

        // center
        Verticies.push_back(0.0f);
        Verticies.push_back(0.0f);
        Verticies.push_back(0.0f);

        // first point
        Verticies.push_back(radius * cos(a0));
        Verticies.push_back(radius * sin(a0));
        Verticies.push_back(0.0f);

        // second point
        Verticies.push_back(radius * cos(a1));
        Verticies.push_back(radius * sin(a1));
        Verticies.push_back(0.0f);

        VertexCount += 3;
    }
}


void Circle::InitVelocity()
{
    // TODO: might move this so each particle doesnt choose its own speed.
    float minSpeed = 0.00005f;
    float maxSpeed = 0.001f;

    // choose angle for the velocity to follow
    float angle = RandomFloat(0.0f, 2.0f * M_PI);

    glm::vec2 dir(cos(angle), sin(angle));

    float speed = RandomFloat(minSpeed, maxSpeed);

    velocity = glm::vec3{(dir * speed), 0.0f};

}


Circle::Circle(int sections, float radius, glm::vec2 pos) 
: PhysicsObject(), SectorCount(sections)
{

    this->radius = radius;
    position = glm::vec3{pos, 0.0f};

    InitVelocity();

    GenerateVerticies();

    // asks opengl to create vao
    glGenVertexArrays(1, &vao);
    // Creates vertex buffer object
    glGenBuffers(1, &vbo);


    // use vertex array provided
    glBindVertexArray(vao);
    // binds vbo and fills it will data
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    
    // static draw tells gpu the data wont change much. 
    glBufferData(GL_ARRAY_BUFFER, (Verticies.size() * sizeof(float)), Verticies.data(), GL_STATIC_DRAW);

    // turn on vert slot zero, usually used for positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,                  // attribute index (matches layout(location = 0) in shader)
        3,                  // number of components per vertex (x, y, z)
        GL_FLOAT,           // type
        GL_FALSE,           // normalize or not
        3 * sizeof(float),  // stride (distance between vertices)
        (void*)0            // offset into the VBO
    );
}

void Circle::GenerateObjectModel()
{
    glm::mat4 model;

    model = glm::mat4(1.0f);

    model = glm::translate(model, position);

    model = glm::rotate(model, rotation.x, {1, 0, 0});
    model = glm::rotate(model, rotation.y, {0, 1, 0});
    model = glm::rotate(model, rotation.z, {0, 0, 1});
    
    model = glm::scale(model, scale);

    this->model = model;
}
