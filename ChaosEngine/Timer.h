#ifndef __TIMER_H__
#define __TIMER_H__

#include "Globals.h"
#include "SDL\include\SDL.h"

class Timer
{
public:

	Timer::Timer(float time = 0.0f) : mTime(time)
	{
		Start();
	}

	void Start();
	void Stop();

	Uint32 Read();

	bool running;


	operator float() const
	{
		return mTime;
	}

	float GetSeconds() const
	{
		return mTime / 10000000.0f;
	}
	float GetMilliseconds() const
	{
		return mTime / 10000.0f;
	}

private:

	Uint32	startedAt;
	Uint32	stoppedAt;
	float mTime;
};

#endif //__TIMER_H__