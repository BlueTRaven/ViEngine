#pragma once

#include "SDL.h"

//Note: in order to maintain any amount of accuraccy, Update should be called all frames.
class ViTimer
{
public:
	enum TimerState
	{
		cTIMER_NONE,
		cTIMER_START,
		cTIMER_PAUSE,
		cTIMER_STOP
	};

	void Start();
	//Temporarily pauses the timer. Call Start() to restart it.
	void Pause();
	//Stops the timer. The timer cannot be restarted, except by re-instantiation.
	void Stop();

	void Update();

	uint32_t GetMs();

	double GetSec();

	inline TimerState GetState() 
	{
		return mState;
	}

private:
	uint32_t msStart;

	uint32_t ms;
	uint32_t dt;

	TimerState mState = TimerState::cTIMER_NONE;
};