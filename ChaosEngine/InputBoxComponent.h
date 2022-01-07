#pragma once

#include "SDL.h"
#include "Component.h"

class SDL_Rect;
class SDL_Color;
class SDL_Texture;

class InputBoxComponent : public Component
{
public:

	InputBoxComponent(int id, const char* text);
	virtual ~InputBoxComponent();

	void Update() override;
	void Draw() override;
	void OnEditor(int i) override;

public:
	SDL_Color color;
	State state;
	std::string text;
	bool drawRect;
};