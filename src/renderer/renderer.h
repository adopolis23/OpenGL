#pragma once

#include "scene.h"
#include "shader.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

class Renderer
{

public:
    Renderer(int w, int h);
    void Render(const Scene& scene);

private:
    int window_width, window_height;

    glm::mat4 view;
    glm::mat4 projection;

    Shader* defaultShader;

};