#pragma once
#include "Globals.h"
#include "Module.h"
#include "glmath.h"
#include <vector>

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
};

class Component
{
public:
	ComponentType type;
	bool active;
	GameObject* owner;
	const char* name;

	virtual void Enable() {}
	virtual void Update() {}
	virtual void Disable() {}
	virtual void OnEditor(int i) {}
};