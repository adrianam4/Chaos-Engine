#pragma once

#include "Component.h"

class SDL_Rect;
class SDL_Color;
class SDL_Texture;

class ImageComponent : public Component
{
public:

	ImageComponent(int id, SDL_Rect bounds, std::string text, SDL_Texture* textureButton);
	~ImageComponent();

	void Update() override;
	void Draw() override;
	void OnEditor(int i) override;

private:
	SDL_Color color;
	SDL_Texture* texture;
	SDL_Rect section;
	State state;
	SDL_Rect bounds;
	std::string text;
	bool buttonsColliders;
};