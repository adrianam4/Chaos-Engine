#pragma once

#include "Component.h"

class SDL_Rect;
class SDL_Color;
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

private:
	SDL_Color color;
	SDL_Texture* texture;
	SDL_Rect section;
	State state;
	SDL_Rect bounds;
	std::string text;
	bool buttonsColliders;
};