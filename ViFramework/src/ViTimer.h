#pragma once

#include "SDL.h"

//Note: in order to maintain any amount of accuraccy, Update should be called all frames.
class ViTimer
{
public:
	void Start();

	void Update();

	uint32_t GetMs();

	double GetSec();

private:
	uint32_t msStart;

	uint32_t ms;
	uint32_t dt;
};