#pragma once
#include "Globals.h"
#include "Module.h"
#include "glmath.h"
#include <vector>
#include "MathGeoLib/src/MathGeoLib.h"

#include "assimp/cimport.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postProcess.h"

#include "Component.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform();
	~ComponentTransform();

	void Enable() override;
	void Update() override;
	void Disable() override;

	float3 position;
	float3 scale;
	Quat rotation;
	//mat4x4 transMatrix;
	//void CalculateTransMatrix(mat4x4 parentsMatrix, mat4x4 localMatrix);
};