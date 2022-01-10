#pragma once

#include "Component.h"
#include "Color.h"

class ImageComponent : public Component
{
public:

	ImageComponent(int id, std::string text);
	~ImageComponent();

	void Update() override;
	void Draw() override;
	void OnEditor(int i) override;

private:
	Color color;
	std::string text;
};