#include "window.h"

Window::Window(const std::string& name, int w, int h, int x, int y)
    :window_name(name), width(w), height(h), x_pos(x), y_pos(y)
{
    if (initialize_sdl() != EXIT_SUCCESS)
    {
        exit(0);
    }

    if (initialize_window() != EXIT_SUCCESS)
    {
        exit(0);
    }

    printf("SDL Window Initialized\n");
}




int Window::initialize_window()
{
    // create window and save pointer to it.
    window = SDL_CreateWindow(
        window_name.c_str(),
        x_pos,
        y_pos,
        width, height,
        SDL_WINDOW_OPENGL
    );

    glContext = SDL_GL_CreateContext(window);

    if (!glContext) {
        printf("Failed to create GL context\n");
        return EXIT_FAILURE;
    }

    printf("GL Context Created\n");

    // Load OpenGL functions with GLAD
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        printf("Failed to load GL\n");
        return EXIT_FAILURE;
    }

    printf("GL Functions Loaded With GLAD\n");

    SDL_GL_SetSwapInterval(1); // vsync

    //main loop stuff to be moved

    // bool running = true;
    // SDL_Event e;

    // while (running) {
    //     while (SDL_PollEvent(&e)) {
    //         if (e.type == SDL_QUIT) running = false;
    //     }

    //     glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    //     glClear(GL_COLOR_BUFFER_BIT);

    //     SDL_GL_SwapWindow(window);
    // }

    // SDL_GL_DeleteContext(GLContext);
    // SDL_DestroyWindow(window);
    // SDL_Quit();


    return EXIT_SUCCESS;
}

// swaps the buffer being shown with the second buffer that theoretically was just rendered to.
void Window::SwapBuffers() {
    SDL_GL_SwapWindow(window);
}

bool Window::PollEvents(SDL_Event& e) {
    // TODO: Look up what this does
    return SDL_PollEvent(&e);
}


Window::~Window()
{
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


int Window::initialize_sdl()
{

    // Call SDL init functions
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL Init Error: " << SDL_GetError() << "\n";
        return EXIT_FAILURE;
    }

    // Request OpenGL 3.3 core (stable and widely supported)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);


    return EXIT_SUCCESS;
}