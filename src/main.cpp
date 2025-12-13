#include<SDL2/SDL.h>
#include<glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include<iostream>

#include "window/window.h"
#include "renderer/renderer.h"
#include "scene/circle.h"
#include "shaders/shader.h"

int main()
{
    Window* window = new Window("Test", 800, 600, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    Renderer* renderer = new Renderer(800, 600);
    Scene scene;

    // simple shader
    Shader shader(
        R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main()
        {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
        }
        )",
        R"(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(0.2, 0.8, 0.3, 1.0);
        }
        )"
    );

    Circle circle(40, 0.1);
    circle.shader = &shader;
    scene.objects.push_back(&circle);

    bool running = true;
    SDL_Event event;

    float angle = 0.0f;

    while (running) {
        while (window->PollEvents(event)) {
            if (event.type == SDL_QUIT)
                running = false;
        }

        circle.GenerateObjectModel();

        renderer->Render(scene);
        window->SwapBuffers();
    }

    return 0;
}