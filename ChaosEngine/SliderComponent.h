#pragma once

#include "UIControl.h"

class SliderComponent : public UIControl
{
public:

	SliderComponent(int id, SDL_Rect bounds, const char* text, SDL_Texture* textureButton, SDL_Texture* textureSlider);
	virtual ~SliderComponent();

	bool Update(float dt);
	bool Draw();

	int value;
	int minValue;
	int maxValue;

private:

	SDL_Rect slider;
	bool drawRect;
};