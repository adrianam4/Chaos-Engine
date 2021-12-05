#pragma once
#include "Globals.h"
#include "SDL\include\SDL.h"

class GameTime
{
private:

	float startedAt = 0.0f;
	float pausedAt = 0.0f;

	bool isActive = false;
	bool isPaused = true;

public:

	GameTime() 
	{ 
		Start(); 
	}
	int Read() const;
	float ReadSec() const;
	bool IsActive() const;
	bool IsPaused() const;

	void Start();
	void Stop();
	void Play();
	void Pause();
};