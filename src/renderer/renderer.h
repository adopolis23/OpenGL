#pragma once

#include "../scene/scene.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

class Renderer
{

public:
    Renderer()
    {
        view = glm::mat4(1.0f); // identity camera
        projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
    }

    void Render(const Scene& scene);

private:

    glm::mat4 view;
    glm::mat4 projection;

};