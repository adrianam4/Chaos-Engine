#pragma once

#include "Component.h"

class SDL_Rect;
class SDL_Color;
class SDL_Texture;

class CheckboxComponent : public Component
{
public:

	CheckboxComponent(int id, const char* text);
	virtual ~CheckboxComponent();

	void Update() override;
	void Draw() override;
	void OnEditor(int i) override;

public:
	SDL_Color color;
	State state;
	std::string text;
	bool drawRectangle;
	bool checked;
};