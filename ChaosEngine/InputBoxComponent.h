#pragma once

#include "UIControl.h"

#include <string>

class InputBoxComponent : public UIControl
{
public:

	InputBoxComponent(int id, SDL_Rect bounds, const char* text, SDL_Texture* textureButton);
	virtual ~InputBoxComponent();

	bool Update(float dt);
	bool Draw();

public:

	bool drawRect;
};