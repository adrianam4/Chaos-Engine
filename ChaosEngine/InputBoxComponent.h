#pragma once

#include "SDL.h"
#include "Component.h"
#include "Color.h"
class Text {
public:
	std::string textt;
	float X, Y, Scale;
	float3 Color;
	void setText(std::string text, float x, float y, float scale, float3 color) {
		textt = text;
		X = x;
		Y = y;
		Scale = scale;
		Color = color;
	}
	void setOnlyText(std::string text) {
		textt = text;
		
	}
	void setOnlyColor(float3 color) {
		Color = color;

	}
};
class InputBoxComponent : public Component
{
public:

	InputBoxComponent(int id, char* text);
	virtual ~InputBoxComponent();

	void Update() override;
	void Draw() override;
	void OnEditor(int i) override;

public:
	Text aux;
	Color color = White;
	Color textColor = Blue;
	State state;
	char text[32] = "Default";
	float fontScale;
	bool drawRect;
};