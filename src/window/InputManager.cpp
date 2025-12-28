#include "InputManager.h"


void InputManager::ProcessEvent(const SDL_Event& e)
{
    switch (e.type)
    {
    case SDL_KEYDOWN:
        if (!e.key.repeat)
            keys_pressed.insert(static_cast<SDL_Scancode>(e.key.keysym.scancode));
        break;

    case SDL_KEYUP:
        keys_pressed.erase(static_cast<SDL_Scancode>(e.key.keysym.scancode));
        break;

    case SDL_MOUSEMOTION:
        mouseX = e.motion.x;
        mouseY = e.motion.y;
        break;

    case SDL_MOUSEBUTTONDOWN:
        // map mouse buttons to SDL_Scancode-like enums or handle separately
		// todo need to implement mouse button handling
        break;

    case SDL_MOUSEBUTTONUP:
        break;

    default:
        break;

    }
}

void InputManager::BeginFrame()
{
    // snapshot previous keys, will be updated during event processing
    prev_keys_pressed = keys_pressed;
}

void InputManager::GetMousePosition(int& x, int& y) const
{
    x = mouseX;
    y = mouseY;
}

void InputManager::EndFrame()
{
	// currently nothing to do here
}

bool InputManager::IsKeyDown(SDL_Scancode sc) const
{
    return keys_pressed.find(sc) != keys_pressed.end();
}

bool InputManager::WasKeyPressed(SDL_Scancode sc) const
{
    return keys_pressed.find(sc) != keys_pressed.end() && prev_keys_pressed.find(sc) == prev_keys_pressed.end();
}