#pragma once
#include "Globals.h"
#include <string>
#include "Module.h"
#include "glmath.h"
#include <vector>
#include "Component.h"
#include"Primitives.h"
#include "MathGeoLib/src/MathGeoLib.h"

class GameObject
{
public:
	void Update();
	Component* CreateComponent(ComponentType type, float3* pos = nullptr, float3* measures = nullptr);
	Component* CreateComponent(ComponentType type, const char* name, bool isDropped);
	int SearchComponent(GameObject* gameObject, ComponentType type);
public:
	
	uint id;
	bool active;
	bool selected;
	std::string name;
	std::vector<Component*> components;
	std::vector<GameObject*> childrens;
	GameObject* parent;
	std::vector<AABB*>aabb;
	std::vector<OBB*>obb;
	bool isChild = false;
	
	float3 trans;
	float3 sca;
	Quat rot;
	float* matrix;
};