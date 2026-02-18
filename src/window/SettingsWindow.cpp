#include "SettingsWindow.h"



SettingsWindow::SettingsWindow(const std::string &name, int w, int h, int x, int y)
    :Window(name, w, h, x, y), renderer(nullptr)
{
    
    printf("Initializing Settings Window.\n");

    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    if (renderer == NULL) {
        printf("Error creating SDL_Renderer!\n");
        return;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    imgui_context = ImGui::CreateContext();
    //ImGui::SetCurrentContext(imgui_context);

    ImGuiIO& io = ImGui::GetIO();

    io.Fonts->AddFontDefault();
    printf("ImGui initialized, io.Fonts->Fonts.Size = %d\n", io.Fonts->Fonts.Size);

     // Setup style
    ImGui::StyleColorsDark();  // Use dark theme
    
    // Setup Platform/Renderer backends
    bool sdl2_init = ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    if (sdl2_init != 1)
    {
        printf("Error: ImGui_ImplSDL2_InitForSDLRenderer failed.\n");
        return;
    }
    bool renderer_init = ImGui_ImplSDLRenderer2_Init(renderer);
    if (sdl2_init != 1 || renderer_init != 1)
    {
        printf("Error: ImGui_ImplSDLRenderer2_Init failed\n");
        return;
    }


    printf("SDL2 backend init: %d, Renderer backend init: %d\n", sdl2_init, renderer_init);


    printf("Settings Window initialized successfully.\n");
}


SettingsWindow::~SettingsWindow()
{
    //Cleanup
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}


void SettingsWindow::CreateUI() {
    // Set initial window position/size (only first time)
    ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_FirstUseEver);
    
    // Create the settings window
    ImGui::Begin(window_name.c_str());
    
    // Add some text
    ImGui::Text("Settings");
    ImGui::Separator();
    
    // Add some sample settings controls
    static float sliderValue = 0.5f;
    ImGui::SliderFloat("Volume", &sliderValue, 0.0f, 1.0f);
    
    static bool checkboxValue = true;
    ImGui::Checkbox("Enable Feature", &checkboxValue);
    
    static int selected = 0;
    const char* options[] = {"Option 1", "Option 2", "Option 3"};
    ImGui::Combo("Options", &selected, options, IM_ARRAYSIZE(options));
    
    if (ImGui::Button("Save Settings")) {
        printf("Settings saved!\n");
    }
    
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
    }
    
    // Show some colored text to verify rendering
    ImGui::TextColored(ImVec4(0, 1, 0, 1), "This text should be green!");
    ImGui::TextColored(ImVec4(1, 0, 0, 1), "This text should be red!");
    
    ImGui::End();
}


void SettingsWindow::Render()
{
    if (!renderer) return;

    ImGuiContext* prev_context = ImGui::GetCurrentContext();


    ImGui::SetCurrentContext(imgui_context);


    // Start ImGui frame
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    
    // Create our UI
    CreateUI();
    
    // Render ImGui
    ImGui::Render();
    
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 45, 45, 45, 255);
    SDL_RenderClear(renderer);
    
    // Render ImGui draw data
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
    
    // Present
    SDL_RenderPresent(renderer);

    
    ImGui::SetCurrentContext(prev_context);
}


// switch to settings window context and process event
void SettingsWindow::HandleEvent(const SDL_Event& event)
{
    ImGuiContext* prev_context = ImGui::GetCurrentContext();
    ImGui::SetCurrentContext(ImGuiContext);

    ImGuiL_ImplSDL2_ProcessEvent(&event);

    ImGui::SetCurrentContext(prev_context);
}







