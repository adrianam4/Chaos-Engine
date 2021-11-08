#pragma once
#include "Globals.h"
#include "Module.h"
#include "glmath.h"
#include <vector>
#include "MathGeoLib/src/MathGeoLib.h"

class GameObject;

enum class ComponentType
{
	TRANSFORM = 1,
	MESH,
	CUBE,
	PYRAMID,
	SPHERE,
	CYLINDER,
	PLANE,
	MATERIAL,
	NONE,
	EMPTY,
};

class Component
{
public:
	ComponentType type;
	bool active;
	bool drawNormals = false;
	bool drawWireframe = false;
	GameObject* owner;
	const char* name;
	const char* modelPath;
	const char* texturePath;
	int width;
	int height;
	u32 UID;
	
	float3 lastRotation;
	float3 lastPosition;
	float3 lastScale;
	float3 position;
	float lastGeneralScale;

	float3 scale;
	float3 rotationEuler;
	float generalScale;
	float4x4 transmat;
	Quat rotationQuat;
		

	virtual void Enable() {}
	virtual void Update() {}
	virtual void Disable() {}
	virtual void Save(const char* texturePath) {}
	virtual void Load(const char* texturePath) {}
	virtual void OnEditor(int i) {}
	u32 GenerateUID();
};