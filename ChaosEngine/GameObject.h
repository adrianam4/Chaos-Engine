#pragma once
#include "Globals.h"
#include <string>
#include "Module.h"
#include "glmath.h"
#include <vector>
#include "Component.h"

class GameObject
{
public:
	void Update();
	Component* CreateComponent(ComponentType type);
public:
	uint id;
	bool active;
	bool selected;
	const char* name;
	std::vector<Component*> components;
	std::vector<GameObject*> childrens;
	GameObject* parent;
};