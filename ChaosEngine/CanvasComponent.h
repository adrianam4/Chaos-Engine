#pragma once
#include "Component.h"
#include "Color.h"

class CanvasComponent : public Component
{
public:

	CanvasComponent(int id, std::string text);
	~CanvasComponent();

	void Update() override;
	void Draw() override;
	void OnEditor(int i) override;

private:
	Color color;
	std::string text;
};