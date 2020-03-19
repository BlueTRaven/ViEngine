#pragma once

#include <SDL.h>

#include "ViUtil.h"

class VIENGINE_EXPORT ViInputManager
{
public:
	void Update();
	void LateUpdate();

	bool KeyDown(SDL_Scancode aKeyCode);
	bool KeyUp(SDL_Scancode aKeyCode);
	bool KeyHeld(SDL_Scancode aKeyCode);

private:
	const uint8_t* mCurrentKeyboardState;
	const uint8_t* mPreviousKeyboardState;
};