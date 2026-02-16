#include "SettingsWindow.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "window.h"
#include <SDL2/SDL_render.h>

SettingsWindow::SettingsWindow(const std::string &name, int w, int h, int x, int y)
    :Window(name, w, h, x, y)
{
    
    printf("Initializing Settings Window.\n");

    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    if (renderer == NULL) {
        printf("Error creating SDL_Renderer!");
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

     // Setup style
    ImGui::StyleColorsDark();  // Use dark theme
    
    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    // Our state variables for UI widgets
    bool show_demo_window = true;
    float slider_value = 0.5f;
    int counter = 0;
    char text_input[256] = "Edit me!";
    bool checkbox_value = false;

    // Main loop
    bool done = false;
    while (!done)
    {
        // Event handling
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && 
                event.window.event == SDL_WINDOWEVENT_CLOSE && 
                event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }
        
        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("My First ImGui Window");  // Begin window
        ImGui::Text("Hello, world!");

        ImGui::End();  // End window
    
        ImGui::Render();
        
        // Clear screen and render ImGui
        SDL_SetRenderDrawColor(renderer, 45, 45, 45, 255);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
        
        done = true;
    }

    // Cleanup
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

