#pragma once

class Timestep
{
public:
	Timestep(float time = 0.0f) : mTime(time)
	{

	}

	float GetSeconds() const 
	{ 
		return mTime; 
	}
	float GetMilliseonds() const 
	{
		return mTime * 1000.0f;
	}

private:
	float mTime;
};