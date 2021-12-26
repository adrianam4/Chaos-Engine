#pragma once

#include "SDL.h"
#include "Component.h"

class SDL_Rect;
class SDL_Color;
class SDL_Texture;

class InputBoxComponent : public Component
{
public:

	InputBoxComponent(int id, SDL_Rect bounds, const char* text, SDL_Texture* textureButton);
	virtual ~InputBoxComponent();

	bool Update(float dt);
	void Draw();

public:
	SDL_Color color;
	SDL_Texture* texture;
	SDL_Rect section;
	State state;
	SDL_Rect bounds;
	std::string text;
	bool drawRect;
};