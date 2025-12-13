#pragma once

#include "scene.h"
#include "shader.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include "globals.h"
#include "camera.h"

class Renderer
{

public:
    Renderer(const Camera* cam);
    void Render(const Scene& scene);
private:

    glm::mat4 view;
    glm::mat4 projection;

    // pointer to external camera object that stores the projetion used for rendering
    const Camera* camera;

    Shader* defaultShader;

};