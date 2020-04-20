#pragma once

#include <SDL.h>

#include "ViUtil.h"

#define INPUT_MANAGER ENVIRONMENT->GetInputManager()

class VIENGINE_EXPORT ViInputManager
{
public:
	enum MouseButton
	{
		cMOUSE_LEFT = SDL_BUTTON(SDL_BUTTON_LEFT),
		cMOUSE_RIGHT = SDL_BUTTON(SDL_BUTTON_RIGHT),
		cMOUSE_MIDDLE = SDL_BUTTON(SDL_BUTTON_MIDDLE)
	};

	ViInputManager();

	void Update();
	void LateUpdate();

	bool KeyDown(SDL_Scancode aKeyCode);
	bool KeyDown(MouseButton aMouseButton);

	bool KeyUp(SDL_Scancode aKeyCode);
	bool KeyUp(MouseButton aMouseButton);

	bool KeyHeld(SDL_Scancode aKeyCode);
	bool KeyHeld(MouseButton aMouseButton);

	vec2i GetMousePosition();
	vec2i GetPreviousMousePosition();
	vec2i GetMouseDelta();
	vec2i GetDistanceFromCenter(SDL_Window* window);

private:
	vec2i mMouseDelta;

	vec2i mCurrentMousePosition;
	vec2i mPreviousMousePosition;
	uint32_t mCurrentMouseState;
	uint32_t mPreviousMouseState;
	const uint8_t* mCurrentKeyboardState;
	const uint8_t* mPreviousKeyboardState;
};