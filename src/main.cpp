#include <SDL2/SDL_events.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <chrono>

#include "window/window.h"
#include "window/SettingsWindow.h"
#include "window/InputManager.h"
#include "renderer/renderer.h"
#include "scene/circle.h"
#include "shaders/shader.h"
#include "renderer/camera.h"
#include "physicsEngine/engine.h"

void InitScene(Scene& scene)
{
    // adds the particles to the simulation, might move to some function in scene maybe?
    int num_particles = 10;
    float radius = 0.01f;
    for (int i = 0; i < num_particles; i++)
    {
        scene.AddObjectToScene(
            // this math just makes a 2d array of objects added to the scene
            new Circle(10, radius, glm::vec2{ -0.9f + (radius * (i % 20)), 0.9f - (radius * (i / 20)) })
        );
    }
}

int main(int argc, char** argv)
{
    int width = 2500;
    int height = 600;

    SettingsWindow* settingsWindow = new SettingsWindow("Settings", 300, height, 100, 100);
    Window* window = new Window("Particle Simulation", width, height, 0, SDL_WINDOWPOS_CENTERED);



    Camera camera(width, height);
    Renderer* renderer = new Renderer(&camera);

    Scene scene;
    InitScene(scene);

	InputManager inputManager;
    Engine* engine = new Engine(&camera, scene);

    settingsWindow->RegisterFloatParameter("Kernel Radius", 0.3f,
        [engine](float initial_value)
        {
            engine->densitySystem.SetKernelRadius(initial_value);
        }); 

    bool running = true;
    SDL_Event event;
    double dt = 0.0f;

    renderer->InitDensityResources(engine->densitySystem.densityField);

    long iter = 0;
	bool paused = false;

    while (running) {
        iter++;

        auto startTime = std::chrono::high_resolution_clock::now();

		inputManager.BeginFrame();
        
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
            
            // Route event to the appropriate window
            Uint32 windowID = SDL_GetWindowID(SDL_GetWindowFromID(event.window.windowID));
            
            if (settingsWindow && windowID == SDL_GetWindowID(settingsWindow->GetWindow())) {
                // This is a settings window event
                settingsWindow->ProcessEvent(event);
            } else if (window && windowID == SDL_GetWindowID(window->GetWindow())) {
                // This is a main window event - handle with input manager
                inputManager.ProcessEvent(event);
            }
        }

        inputManager.EndFrame();



        if (inputManager.IsMouseDown(SDL_BUTTON_LEFT))
        {
			glm::vec2 mousePos = inputManager.GetMousePosition();

			float convertedX = camera.left_world_bound + ((mousePos.x / width) * camera.world_width);
			float convertedY = camera.top_world_bound - ((mousePos.y / height) * camera.world_height);

            engine->ApplyRadialForceToParticlesAtPosition(scene, glm::vec2(convertedX, convertedY), 0.0000003f, 1.0f, dt);
        }
        if (inputManager.IsMouseDown(SDL_BUTTON_RIGHT))
        {
            glm::vec2 mousePos = inputManager.GetMousePosition();

            float convertedX = camera.left_world_bound + ((mousePos.x / width) * camera.world_width);
            float convertedY = camera.top_world_bound - ((mousePos.y / height) * camera.world_height);

            engine->ApplyRadialForceToParticlesAtPosition(scene, glm::vec2(convertedX, convertedY), -0.0000003f, 1.0f, dt);
        }

        if (inputManager.WasKeyPressed(SDL_SCANCODE_SPACE))
            paused = !paused;

        if (!paused)
            engine->Update(scene, dt);

        //renderer->UploadDensity(engine->densityField);
        window->MakeCurrent();
        renderer->UploadParticlePositions(scene, engine->densitySystem.kernelRadius);
        renderer->Render(scene);
        window->SwapBuffers();
        
        // render the secondary settings window
        if (settingsWindow)
        {
            settingsWindow->Render();
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> iterationTime = endTime - startTime;
        std::chrono::duration<double, std::milli> timeInSeconds = iterationTime / 1000;

        dt = iterationTime.count();
        float fps = 1 / timeInSeconds.count();

        if (iter % 200 == 0)
        {
            printf("%f fps\n", fps);
        }
    }

    return 0;
}
