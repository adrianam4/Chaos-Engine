#pragma once
#include "Globals.h"
#include "Module.h"
#include "glmath.h"
#include <vector>

#include "assimp/cimport.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postProcess.h"

#include "Component.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform(float3 pos, float3 sca, Quat rot);
	~ComponentTransform();

	void Enable() override;
	
	void Update() override;
	void Disable() override;
	void OnEditor(int i) override;
	Quat FromEulerToQuat(float3 eulerAngles);
	float3 FromQuatToEuler(Quat quatAngles);
	std::vector<float3> vertices_aux;
	float4x4 transMatrix;
	//mat4x4 transMatrix;
	//void CalculateTransMatrix(mat4x4 parentsMatrix, mat4x4 localMatrix);
};