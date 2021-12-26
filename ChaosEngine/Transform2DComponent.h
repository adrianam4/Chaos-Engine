#pragma once

#include "Globals.h"
#include "Module.h"
#include "glmath.h"
#include <vector>

#include "assimp/cimport.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postProcess.h"
#include "GameObject.h"
class Component;

class ComponentTransform2D : public Component
{
public:
	ComponentTransform2D(GameObject* theObject, float2 pos, float2 sca, float2 rot);
	ComponentTransform2D(float2 pos, float2 sca, float2 rot);
	~ComponentTransform2D();
	ComponentType getComponentType();
	void Enable() override;
	void Update(bool releaseMouse);
	void Disable() override;
	void OnEditor(int i) override;
	void Load(const char* path) override;
	void Save(const char* path) override;
	Quat FromEulerToQuat(float2 eulerAngles);
	float2 FromQuatToEuler(Quat quatAngles);
	/*std::vector<float3> vertices_aux;*/
	float4x4 transMatrix;
	void setOwner();
	//void CreateAABB(ComponentType type, GameObject* go, bool firstTime)override;


	float2 lastRotation;
	float2 lastPosition;
	float2 lastScale;
	float2 position;

	float2 scale;
	float2 rotationEuler;
};
