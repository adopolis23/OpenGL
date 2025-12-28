#pragma once

#include <SDL2/SDL.h>
#include <unordered_set>
#include <glm/gtc/matrix_transform.hpp>

class InputManager
{

public:

	void BeginFrame();
	void ProcessEvent(const SDL_Event& e); // call for every SDL_Event polled
	void EndFrame();

	glm::vec2 GetMousePosition() const;
	bool WasKeyPressed(SDL_Scancode sc) const;
	bool IsKeyDown(SDL_Scancode sc) const;

	bool IsMouseDown(Uint8 button) const;                    // currently held down
	bool WasMouseClicked(Uint8 button) const;                // pressed this frame
	glm::vec2 GetLastClickPosition() const;         // position of the most recent click (frame-local)


private:

	std::unordered_set<SDL_Scancode> keys_pressed;
	std::unordered_set<SDL_Scancode> prev_keys_pressed;

	std::unordered_set<Uint8> mouse_down;        // buttons currently down
	std::unordered_set<Uint8> prev_mouse_down;   // buttons down last frame

	int mouseX = 0;
	int mouseY = 0;

	// data local to current frame
	bool mouseClicked = false;
	Uint8 mouseClickedButton = 0;
	int mouseClickX = 0;
	int mouseClickY = 0;

};
