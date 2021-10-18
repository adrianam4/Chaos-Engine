#pragma once
#include "Globals.h"
#include "Module.h"
#include "glmath.h"
#include <vector>

#include "Component.h"

class ComponentMesh : public Component
{
public:
	ComponentMesh();
	~ComponentMesh();

	void Enable() override;
	void Update() override;
	void Disable() override;
};