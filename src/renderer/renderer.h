#pragma once

#include "scene.h"
#include "shader.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include "globals.h"

class Renderer
{

public:
    Renderer(int w, int h);
    void Render(const Scene& scene);

    float aspect_ratio;

    float world_height;
    float world_width;

    float left;
    float right;
    float top;
    float bottom;

private:
    int window_width, window_height;

    glm::mat4 view;
    glm::mat4 projection;

    Shader* defaultShader;

};