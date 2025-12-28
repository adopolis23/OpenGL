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
        mouse_down.insert(e.button.button);
        mouseX = e.button.x;
        mouseY = e.button.y;
        mouseClicked = true;
        mouseClickedButton = e.button.button;
        mouseClickX = e.button.x;
        mouseClickY = e.button.y;
        break;

    case SDL_MOUSEBUTTONUP:
        mouse_down.erase(e.button.button);
        mouseX = e.button.x;
        mouseY = e.button.y;
        break;

    default:
        break;

    }
}

void InputManager::BeginFrame()
{
    // snapshot previous keys, will be updated during event processing
    prev_keys_pressed = keys_pressed;
    prev_mouse_down = mouse_down;

    // clear per-frame click flag at start of frame
    mouseClicked = false;
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

// mouse functions

glm::vec2 InputManager::GetMousePosition() const
{
    return glm::vec2(mouseX, mouseY);
}

bool InputManager::IsMouseDown(Uint8 button) const
{
    return mouse_down.find(button) != mouse_down.end();
}

bool InputManager::WasMouseClicked(Uint8 button) const
{
    // true if a click event for this button occurred this frame
    return mouseClicked && mouseClickedButton == button;
}

glm::vec2 InputManager::GetLastClickPosition() const
{
    return glm::vec2(mouseClickX, mouseClickY);
}