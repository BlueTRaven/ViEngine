#include "ViInputManager.h"

void ViInputManager::Update()
{
	SDL_PumpEvents();
	mCurrentKeyboardState = SDL_GetKeyboardState(NULL);
}

void ViInputManager::LateUpdate()
{
	mPreviousKeyboardState = mCurrentKeyboardState;
}

bool ViInputManager::KeyDown(SDL_Scancode aKeyCode)
{
	return mCurrentKeyboardState[aKeyCode] && !mPreviousKeyboardState[aKeyCode];
}

bool ViInputManager::KeyUp(SDL_Scancode aKeyCode)
{
	return !mCurrentKeyboardState[aKeyCode];
}

bool ViInputManager::KeyHeld(SDL_Scancode aKeyCode)
{
	return mCurrentKeyboardState[aKeyCode];
}
