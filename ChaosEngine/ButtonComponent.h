#pragma once

#include "Component.h"
#include "Color.h"

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

public:
	Color disabledColor = White;
	Color pressedColor = Red;
	Color focusedColor = Blue;
	Color normalColor = Green;

	State state;
	std::string text;
};