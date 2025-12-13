#pragma once

#include "../scene/scene.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

class Renderer
{

public:
    Renderer(int w, int h)
    : window_width(w), window_height(h)
    {
        view = glm::mat4(1.0f); // identity camera

        float aspect_ratio = (float)window_width / (float)window_height;

        projection = glm::ortho(-aspect_ratio, aspect_ratio, -1.0f, 1.0f);
    }

    void Render(const Scene& scene);

private:
    int window_width, window_height;

    glm::mat4 view;
    glm::mat4 projection;

};