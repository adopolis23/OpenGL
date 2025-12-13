#include<SDL2/SDL.h>
#include<glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include<iostream>

#include "window/window.h"
#include "renderer/renderer.h"
#include "scene/circle.h"
#include "shaders/shader.h"
#include "renderer/camera.h"

int main()
{
    Window* window = new Window("Test", 800, 600, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    Camera camera(800, 600);
    Renderer* renderer = new Renderer(&camera);
    Scene scene;


    Circle circle(40, 0.1);
    scene.objects.push_back(&circle);

    bool running = true;
    SDL_Event event;

    while (running) {
        while (window->PollEvents(event)) {
            if (event.type == SDL_QUIT)
                running = false;
        }

        circle.position.x = camera.right_world_bound - 0.1f;

        renderer->Render(scene);
        window->SwapBuffers();
    }

    return 0;
}