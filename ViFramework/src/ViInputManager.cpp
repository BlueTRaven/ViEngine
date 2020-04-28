#include "ViInputManager.h"

ViInputManager::ViInputManager() : 
	mPreviousKeyboardState((uint8_t*)malloc(SDL_NUM_SCANCODES))
{
}

void ViInputManager::Update()
{
	SDL_PumpEvents();
	int x = 0, y = 0;
	mCurrentMouseState = SDL_GetMouseState(&x, &y);
	mCurrentMousePosition = { x, y };
	mMouseDelta = mPreviousMousePosition - mCurrentMousePosition;

	mCurrentKeyboardState = SDL_GetKeyboardState(NULL);
}

void ViInputManager::LateUpdate()
{
	mPreviousMouseState = mCurrentMouseState;
	mPreviousMousePosition = mCurrentMousePosition;
	
	//If we just assign mPreviousKeyboardState to mCurrentKeyboardState, they'll share pointers, so when we set mCurrentKeyboardState above, it also overwrites the previous.
	//Note that it copies to mPreviousKeyboardState's pointer, so we actually have to have a pointer. Thus the constructor.
	memcpy((void*)mPreviousKeyboardState, mCurrentKeyboardState, SDL_NUM_SCANCODES);
}

bool ViInputManager::KeyDown(SDL_Scancode aKeyCode)
{	
	return mCurrentKeyboardState[aKeyCode] && !mPreviousKeyboardState[aKeyCode];
}

bool ViInputManager::KeyDown(MouseButton aMouseButton)
{
	return (mCurrentMouseState & aMouseButton) && !(mPreviousMouseState & aMouseButton);
}

bool ViInputManager::KeyUp(SDL_Scancode aKeyCode)
{
	return !mCurrentKeyboardState[aKeyCode];
}

bool ViInputManager::KeyUp(MouseButton aMouseButton)
{
	return !(mCurrentMouseState & aMouseButton);
}

bool ViInputManager::KeyHeld(SDL_Scancode aKeyCode)
{
	return mCurrentKeyboardState[aKeyCode];
}

bool ViInputManager::KeyHeld(MouseButton aMouseButton)
{
	return mCurrentMouseState & aMouseButton;
}

vec2i ViInputManager::GetMousePosition()
{
	return mCurrentMousePosition;
}

vec2i ViInputManager::GetPreviousMousePosition()
{
	return mPreviousMousePosition;
}

vec2i ViInputManager::GetMouseDelta()
{
	return mMouseDelta;
}

vec2i ViInputManager::GetDistanceFromCenter(SDL_Window* window)
{
	int width = 0, height = 0;
	SDL_GetWindowSize(window, &width, &height);

	return vec2i(width / 2, height / 2) - mCurrentMousePosition;
}
