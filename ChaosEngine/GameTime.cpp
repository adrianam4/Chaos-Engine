#include "GameTime.h"

bool GameTime::IsActive() const
{
	return isActive;
}

bool GameTime::IsPaused() const
{
	return isPaused;
}

int GameTime::Read() const
{
	if (!isActive)
		return 0;

	if (!isPaused)
		return (SDL_GetTicks() - startedAt);
	else
		return (pausedAt - startedAt);
}

float GameTime::ReadSec() const
{
	if (!isActive)
		return 0.0f;

	if (!isPaused)
		return (float)((SDL_GetTicks() - startedAt) / 1000.0f);
	else
		return (float)((pausedAt - startedAt) / 1000.0f);
}

void GameTime::Start()
{
	startedAt = SDL_GetTicks();
	pausedAt = 0.0f;
	isActive = true;
	isPaused = false;
}

void GameTime::Stop()
{
	startedAt = 0.0f;
	pausedAt = 0.0f;
	isActive = false;
	isPaused = true;
}

void GameTime::Play()
{
	isPaused = false;
	startedAt = (SDL_GetTicks() + startedAt) - pausedAt;
}

void GameTime::Pause()
{
	if (isPaused)
	{
		return;
	}

	isPaused = true;
	pausedAt = SDL_GetTicks();
}