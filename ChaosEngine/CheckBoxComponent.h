#pragma once

#include "UIControl.h"

class CheckboxComponent : public UIControl
{
public:

	CheckboxComponent(int id, SDL_Rect bounds, const char* text, SDL_Texture* textureButton);
	virtual ~CheckboxComponent();

	bool Update(float dt);
	bool Draw();

public:
	
	bool drawRectangle;
	bool checked;
};