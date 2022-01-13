#pragma once

#include "Component.h"
#include "Color.h"
#include "Text.h"

class SDL_Rect;
class SDL_Texture;

class ButtonComponent : public Component
{
public:

	ButtonComponent(int id, std::string text);
	~ButtonComponent();

	void Update() override;
	void Draw() override;
	void OnEditor(int i) override;
	void OnClick() override;

	float2 GetParentPosition();

public:
	Text buttonText;

	Color disabledColor = White;
	Color pressedColor = Red;
	Color focusedColor = Blue;
	Color normalColor = Green;
	Color selectedColor = Black;

	Color textColor = White;
	char text[32] = "Button";
	float fontScale = 1;
};