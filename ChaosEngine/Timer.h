#ifndef __TIMER_H__
#define __TIMER_H__

#include "Globals.h"
#include "SDL\include\SDL.h"
#include <chrono>

class Timer
{
public:

	Timer::Timer(float time = 0.0f) : rTime(time)
	{
		Start();
	}

	void Start();
	void Stop();

	Uint32 Read();

	bool running;


	operator float() const
	{
		return rTime;
	}

	float GetSeconds() const
	{
		return rTime / 10000000.0f;
	}
	float GetMilliseconds() const
	{
		return rTime / 10000.0f;
	}

private:

	Uint32	startedAt;
	Uint32	stoppedAt;
	float rTime;
};

#endif //__TIMER_H__