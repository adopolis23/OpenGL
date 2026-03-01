// SettingsWindow.h
#pragma once
#include <functional>
#include <unordered_map>
#include <string>

#include "window.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"


class SettingsWindow : public Window {

public:
    SettingsWindow(const char* title, int w, int h, int x, int y);

    void RegisterFloatParameter(std::string param_name, float initial_value, std::function<void(float)> callback);
    void RegisterIntParameter(std::string param_name, int initial_value, std::function<void(int)> callback);

    virtual ~SettingsWindow();
    
    void Render();
    
    void ProcessEvent(SDL_Event& event);
    
    void CreateMainSettingsWindow();


private:
    // callback functions for the settings window to use on value change
    std::unordered_map<std::string, std::function<void(float)>> floatCallbacks;
    std::unordered_map<std::string, std::function<void(int)>> intCallbacks;

    // values for the parameters
    std::unordered_map<std::string, float> floatValues;
    std::unordered_map<std::string, int> intValues;

};
