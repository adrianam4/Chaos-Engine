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
	ComponentMesh(std::vector<theBuffer*>* theArray);
	ComponentMesh(ComponentType mType, float3* pos=nullptr, float3* measures = nullptr);
	ComponentMesh(ComponentType mType, char* name);
	~ComponentMesh();

	void Enable() override;
	void Update() override;
	void Disable() override;
	void OnEditor(int i) override;
	void Load(const char* path) override;
	void Save(const char* path) override;
};