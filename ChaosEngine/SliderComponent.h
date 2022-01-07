#pragma once

#include "Component.h"

class SDL_Rect;
class SDL_Color;
class SDL_Texture;

class SliderComponent : public Component
{
public:

	SliderComponent(int id, const char* text);
	virtual ~SliderComponent();

	void Update() override;
	void Draw() override;
	void OnEditor(int i) override;

	float value;
	float minValue;
	float maxValue;
	MyPlane* thePlane;
	bool completed;
	float barProgres;
private:
	SDL_Color color;
	State state;
	std::string text;
	SDL_Rect slider;
	bool drawRect;
};