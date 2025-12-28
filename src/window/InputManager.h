#pragma once

#include <SDL2/SDL.h>
#include <unordered_set>

class InputManager
{

public:

	void BeginFrame();
	void ProcessEvent(const SDL_Event& e); // call for every SDL_Event polled
	void EndFrame();

	void GetMousePosition(int& x, int& y) const;
	bool WasKeyPressed(SDL_Scancode sc) const;
	bool IsKeyDown(SDL_Scancode sc) const;

private:

	std::unordered_set<SDL_Scancode> keys_pressed;
	std::unordered_set<SDL_Scancode> prev_keys_pressed;

	int mouseX = 0;
	int mouseY = 0;
};
