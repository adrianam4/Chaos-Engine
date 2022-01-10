#pragma once

#include "Component.h"
#include "Color.h"

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
	Color disabledColor = White;
	Color pressedColor = Red;
	Color focusedColor = Blue;
	Color normalColor = Green;
	Color selectedColor = Black;

	std::string text;
	bool drawRect;
};