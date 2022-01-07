#pragma once

#include "Component.h"
#include "Color.h"

class CheckboxComponent : public Component
{
public:

	CheckboxComponent(int id, const char* text);
	virtual ~CheckboxComponent();

	void Update() override;
	void Draw() override;
	void OnEditor(int i) override;
	void OnClick() override;

public:
	Color disabledColor = White;
	Color pressedColor = Red;
	Color focusedColor = Blue;
	Color normalColor = Green;

	State state;
	std::string text;
	bool checked;
};