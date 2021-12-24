#pragma once

#include "UIControl.h"

class ButtonComponent : public UIControl
{
public:

	ButtonComponent(int id, SDL_Rect bounds, const char* text, SDL_Texture* textureButton);
	~ButtonComponent();

	bool Update(float dt);
	bool Draw()override;

private:

	bool buttonsColliders;
};