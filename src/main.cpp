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

    // adds the particles to the simulation, might move to some function in scene maybe?
    int num_particles = 30;
    float radius = 0.02f;
    for (int i = 0; i < num_particles; i++)
    {
        scene.objects.push_back(
            // this math just makes a 2d array of objects added to the scene
            new Circle(20, radius, glm::vec2{0.0f + (radius * 4 * (i % 10)), 0.0f - (radius * 4 * (i / 10))})
        );
    }



    bool running = true;
    SDL_Event event;

    while (running) {
        while (window->PollEvents(event)) {
            if (event.type == SDL_QUIT)
                running = false;
        }

        renderer->Render(scene);
        window->SwapBuffers();
    }

    return 0;
}