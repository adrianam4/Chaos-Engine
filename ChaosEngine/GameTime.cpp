#include "GameTime.h"
#include "mmgr.h"

void GameTime::Start()
{
	isActive = true;
	isPaused = false;
	startedAt = SDL_GetTicks();
	pausedAt = 0.0f;
}

void GameTime::Stop()
{
	isActive = false;
	isPaused = true;
	startedAt = 0.0f;
	pausedAt = 0.0f;
}

void GameTime::Play()
{
	if(isPaused)
		return;

	isPaused = false;
	startedAt = (SDL_GetTicks() + startedAt) - pausedAt;
}

void GameTime::Pause()
{
	if (isPaused)
		return;

	isPaused = true;
	pausedAt = SDL_GetTicks();
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