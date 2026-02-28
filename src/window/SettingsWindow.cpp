#include "SettingsWindow.h"
#include <SDL2/SDL_events.h>



SettingsWindow::SettingsWindow(const char* title, int w, int h, int x, int y) : Window(title, w, h, x, y) 
{
        
        // Setup ImGui for this window
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        
        // Setup Platform/Renderer bindings
        ImGui_ImplSDL2_InitForOpenGL(window, glContext);
        ImGui_ImplOpenGL3_Init("#version 130");
        
        // Make this window current for ImGui initialization
        MakeCurrent();

        printf("SettingsWindow ImGui Initialized.");
    }


SettingsWindow::~SettingsWindow() 
{
    // Cleanup ImGui
    MakeCurrent(); // Ensure we're on the right context
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}


void SettingsWindow::Render()
{
    // Make this window's context current
    MakeCurrent();
    
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    
    // Create ImGui windows
    CreateMainSettingsWindow();
    
    // Rendering
    ImGui::Render();
    glViewport(0, 0, width, height);
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    // Swap buffers
    SwapBuffers();
}

void SettingsWindow::ProcessEvent(SDL_Event& event)
{
    // Let ImGui handle the event first
    ImGui_ImplSDL2_ProcessEvent(&event);
    
    // If ImGui doesn't want to handle it, you can add your own handling
    if (!ImGui::GetIO().WantCaptureMouse && 
        !ImGui::GetIO().WantCaptureKeyboard) {
        // Handle events that shouldn't be captured by ImGui
        if (event.type == SDL_KEYDOWN) {
            // Your custom keyboard handling here
        }
    }
}


void SettingsWindow::CreateMainSettingsWindow()
{
    // Set the next window to cover the entire SDL window
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(width, height));
    
    // Window flags to make it non-movable, non-resizable, and without title bar
    ImGuiWindowFlags windowFlags = 
        ImGuiWindowFlags_NoMove |              // Cannot be moved
        ImGuiWindowFlags_NoResize |             // Cannot be resized
        ImGuiWindowFlags_NoTitleBar |           // No title bar
        ImGuiWindowFlags_NoCollapse |           // Cannot collapse
        ImGuiWindowFlags_NoBringToFrontOnFocus | // Stays in background
        ImGuiWindowFlags_NoNavFocus;            // No navigation focus

    // Create a settings window
    ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    
    ImGui::Text("Simulation Controls");
    ImGui::Separator();
    
    ImGui::SliderFloat("Slider", &sliderValue, 0.0f, 1.0f, "%.3f");
    
    ImGui::End();
}
