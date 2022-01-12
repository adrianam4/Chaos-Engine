#pragma once

#include "SDL.h"
#include "Component.h"
#include "Color.h"
class Text {
public:
	std::string textt;
	float X, Y, Scale;
	float3 Color;

	void setText(std::string text, float x, float y, float scale, float3 color) 
	{
		textt = text;
		X = x;
		Y = y;
		Scale = scale;
		Color = color;
	}
	void setOnlyText(std::string text) 
	{
		textt = text;
	}
	void setOnlyColor(float3 color) 
	{
		Color = color;
	}
	void SetOnlyPosition(float2 pos)
	{
		X = pos.x;
		Y = pos.y;
	}
};
class InputBoxComponent : public Component
{
public:

	InputBoxComponent(int id, std::string text);
	virtual ~InputBoxComponent();

	void Update() override;
	void Draw() override;
	void OnEditor(int i) override;
	void OnClick() override;
	float2 GetParentPosition();

public:
	Text aux;

	Color disabledColor = White;
	Color pressedColor = Red;
	Color focusedColor = Blue;
	Color normalColor = Green;
	Color selectedColor = Black;

	Color color = normalColor;
	Color textColor = Blue;

	char text[32] = "Introduce your name...";
	float fontScale = 1;
	bool drawRect;
};