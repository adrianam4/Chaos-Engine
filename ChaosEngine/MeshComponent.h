#pragma once
#include "Globals.h"
#include "Module.h"
#include "glmath.h"
#include <vector>

#include "Component.h"

enum class MeshType
{
	MODEL = 1,
	CUBE,
	PYRAMID,
	SPHERE,
	CYLINDER,
};

class ComponentMesh : public Component
{
public:
	ComponentMesh(ComponentType mType);
	ComponentMesh(ComponentType mType, char* name);
	~ComponentMesh();

	void Enable() override;
	void Update() override;
	void Disable() override;
};