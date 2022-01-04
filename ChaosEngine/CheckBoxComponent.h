#pragma once

#include "Component.h"

class SDL_Rect;
class SDL_Color;
class SDL_Texture;

class CheckboxComponent : public Component
{
public:

	CheckboxComponent(int id, SDL_Rect bounds, const char* text, SDL_Texture* textureButton);
	virtual ~CheckboxComponent();

	void Update() override;
	void Draw() override;
	void OnEditor(int i) override;

public:
	SDL_Color color;
	SDL_Texture* texture;
	SDL_Rect section;
	State state;
	SDL_Rect bounds;
	std::string text;
	bool drawRectangle;
	bool checked;
};