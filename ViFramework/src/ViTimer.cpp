#include "ViTimer.h"

void ViTimer::Start()
{
	if (mState == TimerState::cTIMER_STOP)
		return;

	msStart = SDL_GetTicks();

	mState = TimerState::cTIMER_START;
}

void ViTimer::Pause()
{
	mState = TimerState::cTIMER_PAUSE;
}

void ViTimer::Stop()
{
	mState = cTIMER_STOP;
}

void ViTimer::Update()
{
	if (mState == TimerState::cTIMER_PAUSE || mState == TimerState::cTIMER_STOP)
		return;

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
