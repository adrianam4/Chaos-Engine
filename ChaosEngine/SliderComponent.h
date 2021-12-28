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

	void Update() override;
	void Draw() override;
	void OnEditor(int i) override;

	float value;
	float minValue;
	float maxValue;

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