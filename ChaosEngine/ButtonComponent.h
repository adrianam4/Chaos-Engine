#pragma once

#include "Component.h"
#include "Color.h"

class SDL_Rect;
class SDL_Texture;

class ButtonComponent : public Component
{
public:

	ButtonComponent(int id, SDL_Rect bounds, std::string text, SDL_Texture* textureButton);
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

	SDL_Texture* disabledTexture = nullptr;
	SDL_Texture* pressedTexture = nullptr;
	SDL_Texture* focusedTexture = nullptr;
	SDL_Texture* normalTexture = nullptr;

	SDL_Rect* section = nullptr;
	State state;
	SDL_Rect* bounds = nullptr;
	std::string text;
};