// SettingsWindow.h
#pragma once
#include "window.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

class SettingsWindow : public Window {
private:
    float sliderValue = 0.5f;
    
public:
    SettingsWindow(const char* title, int w, int h, int x, int y);


    virtual ~SettingsWindow();
    
    void Render();
    
    void ProcessEvent(SDL_Event& event);
    
    void CreateMainSettingsWindow();
    
    // Getters for settings values (to be used by main application)
    float GetSliderValue() const { return sliderValue; }
};
