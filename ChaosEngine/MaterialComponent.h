#pragma once
#include "Globals.h"
#include "Module.h"
#include "glmath.h"
#include <vector>

#include "Component.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial();
	~ComponentMaterial();

	void Enable() override;
	void Update() override;
	void Disable() override;
	void OnEditor(int i) override;
};