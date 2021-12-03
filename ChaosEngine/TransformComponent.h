#pragma once
#include "Globals.h"
#include "Module.h"
#include "glmath.h"
#include <vector>

#include "assimp/cimport.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postProcess.h"

class Component;

class ComponentTransform : public Component
{
public:
	ComponentTransform(GameObject* theObject, float3 pos, float3 sca, float3 rot);
	ComponentTransform(float3 pos, float3 sca, float3 rot);
	~ComponentTransform();
	ComponentType getComponentType();
	void Enable() override;
	void Update() override;
	void Disable() override;
	void OnEditor(int i) override;
	void Load(const char* path) override;
	void Save(const char* path) override;
	Quat FromEulerToQuat(float3 eulerAngles);
	float3 FromQuatToEuler(Quat quatAngles);
	std::vector<float3> vertices_aux;
	float4x4 transMatrix;
	void setOwner();
	void CreateAABB(ComponentType type, GameObject* go, bool firstTime)override;
	
};