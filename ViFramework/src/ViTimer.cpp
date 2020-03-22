#include "ViTimer.h"

void ViTimer::Start()
{
	msStart = SDL_GetTicks();
}

void ViTimer::Update()
{
	//We want our ms to be relative - SDL_GetTicks returns a global timer from when SDL was first initialized, so we have to make it relative here.
	uint32_t sdlms = SDL_GetTicks();
	ms = sdlms - msStart;
}

uint32_t ViTimer::GetMs()
{
	return ms;
}

double ViTimer::GetSec()
{
	return (float)ms / 1000.f;
}
