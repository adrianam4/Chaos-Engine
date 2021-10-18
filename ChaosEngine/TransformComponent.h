#pragma once
#include "Globals.h"
#include "Module.h"
#include "glmath.h"
#include <vector>

#include "Component.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform();
	~ComponentTransform();

	void Enable() override;
	void Update() override;
	void Disable() override;
};