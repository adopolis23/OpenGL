#pragma once

#include "window.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>

class SettingsWindow : public Window {

public:
    SettingsWindow(const std::string &name, int w, int h, int x, int y);

    ~SettingsWindow();

    void HandleEvent(const SDL_Event& event);

    void CreateUI();
    
    void Render();

private:
    SDL_Renderer* renderer;

};
