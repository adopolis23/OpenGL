#include "circle.h"
#include <iostream>

void Circle::GenerateVerticies()
{
    SectorCount = 20;

    float theta_radians = (SectorCount / 360) * (2*M_PI / 360);

    for (int sector = 0; sector <= SectorCount; sector++)
    {

        // add center vert
        Verticies.push_back(0.0f);
        Verticies.push_back(0.0f);
        Verticies.push_back(0.0f);

        float angle_first_line = theta_radians * sector;
        float angle_second_line = theta_radians * (sector + 1);

        // add second vert
        float x_first = cos(angle_first_line);
        float y_first = sin(angle_first_line);
        Verticies.push_back(x_first);
        Verticies.push_back(y_first);
        Verticies.push_back(0.0f);


        // add third vert
        float x_second = cos(angle_second_line);
        float y_second = sin(angle_second_line);
        Verticies.push_back(x_second);
        Verticies.push_back(y_second);
        Verticies.push_back(0.0f);

        VertexCount += 9;


    }

    std::cout << Verticies.data() << std::endl;
    
}


Circle::Circle() 
{

    GenerateVerticies();

    // asks opengl to create vao
    glGenVertexArrays(1, &vao);
    // Creates vertex buffer object
    glGenBuffers(1, &vbo);


    // used vertex array provided
    glBindVertexArray(vao);
    // binds vbo and fills it will data
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    
    // static draw tells gpu the data wont change much. 
    glBufferData(GL_ARRAY_BUFFER, sizeof(Verticies), Verticies.data(), GL_STATIC_DRAW);

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
