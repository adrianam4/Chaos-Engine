#pragma once

#include "Component.h"

class SDL_Rect;
class SDL_Color;
class SDL_Texture;

class SliderComponent : public Component
{
public:

	SliderComponent(int id, SDL_Rect bounds, const char* text, SDL_Texture* textureButton, SDL_Texture* textureSlider);
	virtual ~SliderComponent();

	bool Update(float dt);
	void Draw();

	int value;
	int minValue;
	int maxValue;

private:
	SDL_Color color;
	SDL_Texture* texture;
	SDL_Rect section;
	State state;
	SDL_Rect bounds;
	std::string text;
	SDL_Rect slider;
	bool drawRect;
};