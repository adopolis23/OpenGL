#pragma once

#include <globals.h>
#include<SDL2/SDL.h>
#include<glad/glad.h>
#include<iostream>
#include <string>

class Window
{

public:
    Window(const std::string& name, int w, int h, int x, int y);
    ~Window();


    bool PollEvents(SDL_Event& e);

    void SwapBuffers();
    
private:

    // both of these are called from inside the contructor
    int initialize_sdl();
    int initialize_window();

    std::string window_name;
    int width, height;
    int x_pos, y_pos;

    SDL_Window* window = nullptr;

    // This is where all the GPU rendering info is (apparently)
    SDL_GLContext glContext;
};