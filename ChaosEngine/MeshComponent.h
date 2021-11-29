#pragma once
#include "Globals.h"
#include "Module.h"
#include "glmath.h"
#include <vector>

#include "Mesh.h"
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
	ComponentMesh(std::vector<theBuffer*>* theArray , const char* mName);
	ComponentMesh(ComponentType mType, float3* pos=nullptr, float3* measures = nullptr);
	
	~ComponentMesh();

	void Enable() override;
	void Update() override;
	void Disable() override;
	void OnEditor(int i) override;
	void Load(const char* path) override;
	void Save(const char* path) override;
};