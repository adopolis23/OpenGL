#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <chrono>

#include "window/window.h"
#include "renderer/renderer.h"
#include "scene/circle.h"
#include "shaders/shader.h"
#include "renderer/camera.h"
#include "physicsEngine/engine.h"

int main()
{
    Window* window = new Window("Test", 800, 800, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    Camera camera(800, 600);
    Renderer* renderer = new Renderer(&camera);
    Engine* engine = new Engine(&camera);
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
    double dt = 0.0f;

    renderer->InitDensityResources(engine->densityField);

    while (running) {

        auto startTime = std::chrono::high_resolution_clock::now();

        while (window->PollEvents(event)) {
            if (event.type == SDL_QUIT)
                running = false;
        }

        engine->Update(scene, dt);
        renderer->UploadDensity(engine->densityField);
        renderer->Render(scene);
        window->SwapBuffers();

        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> iterationTime = endTime - startTime;
        std::chrono::duration<double, std::milli> timeInSeconds = iterationTime / 1000;

        dt = iterationTime.count();
        float fps = 1 / timeInSeconds.count();
        printf("%f fps\n", fps);
    }

    return 0;
}