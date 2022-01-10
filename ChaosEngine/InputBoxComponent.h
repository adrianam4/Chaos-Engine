#pragma once

#include "SDL.h"
#include "Component.h"
#include "Color.h"

class InputBoxComponent : public Component
{
public:

	InputBoxComponent(int id, char* text);
	virtual ~InputBoxComponent();

	void Update() override;
	void Draw() override;
	void OnEditor(int i) override;

public:
	Color color = White;
	Color textColor = Blue;
	State state;
	char text[32] = "Default";
	float fontScale;
	bool drawRect;
};