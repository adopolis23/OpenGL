#include "densityBackground.h"

void DensityBackground::GenerateVerticies()
{
    float quadVerts[] = {
        // pos      // uv
        -1, -1,     0, 0,
         1, -1,     1, 0,
         1,  1,     1, 1,

        -1, -1,     0, 0,
         1,  1,     1, 1,
        -1,  1,     0, 1
    };

    for (float f : quadVerts)
    {
        Verticies.push_back(f);
    }

    VertexCount = 6;

}

void DensityBackground::GenerateObjectModel()
{
    
}

DensityBackground::DensityBackground()
{
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

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


}