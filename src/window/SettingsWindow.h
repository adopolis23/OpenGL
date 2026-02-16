#pragma once

#include "window.h"
#include <SDL2/SDL_render.h>

class SettingsWindow : public Window {

public:
    SettingsWindow(const std::string &name, int w, int h, int x, int y);

private:
    SDL_Renderer* renderer;

};
