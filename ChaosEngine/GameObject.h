#pragma once
#include "Globals.h"
#include <string>
#include "Module.h"
#include "glmath.h"
#include <vector>
#include "Component.h"

#include "MathGeoLib/src/MathGeoLib.h"

class GameObject
{
public:
	void Update();
	Component* CreateComponent(ComponentType type);
	Component* CreateComponent(ComponentType type, const char* name);
	int SearchComponent(GameObject* gameObject, ComponentType type);
public:
	uint id;
	bool active;
	bool selected;
	const char* name;
	std::vector<Component*> components;
	std::vector<GameObject*> childrens;
	GameObject* parent;

	float3 trans;
	float3 sca;
	Quat rot;
	float* matrix;
};