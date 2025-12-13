#pragma once

#include "../scene/scene.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Renderer
{

public:
    Renderer()
    {
        
    }

    void Render(const Scene& scene);

private:

};