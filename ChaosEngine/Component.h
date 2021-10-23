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
	MATERIAL,
	EMPTY,
};

class Component
{
public:
	ComponentType type;
	bool active;
	GameObject* owner;
	const char* name;

	float3 position;
	float3 scale;
	float3 rotationEuler;
	Quat rotationQuat;

	virtual void Enable() {}
	virtual void Update() {}
	virtual void Disable() {}
	virtual void OnEditor(int i) {}
};