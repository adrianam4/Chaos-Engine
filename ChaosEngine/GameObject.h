#pragma once
#include "Globals.h"
#include <string>
#include "Module.h"
#include "glmath.h"
#include <vector>
#include "Component.h"
#include"Primitives.h"
#include"Importer.h"
#include"TransformComponent.h"

class GameObject
{
public:
	void Update();
	Component* CreateComponent(ComponentType type, float3* pos = nullptr, float3* measures = nullptr, float3* rotation = nullptr);
	Component* CreateComponent(ComponentType type, const char* name, bool isDropped);
	int SearchComponent(GameObject* gameObject, ComponentType type);
	u32 GenerateUID();
	Component* CreateMeshComponent(std::vector<theBuffer*>* theArray,const char* path);
public:
	std::vector<float3> GetVertices(int id);
	std::vector<float3> GetNormals(int id);

	u32 UID;
	u32 parentUID;
	uint id;
	bool active;
	bool selected;
	std::string name;
	std::vector<Component*> components;
	std::vector<GameObject*> childrens;
	std::vector<BoundingBoxes*> boundingBoxes;
	GameObject* parent;
	std::vector<AABB*>aabb;
	std::vector<OBB*>obb;
	bool isChild = false;
	ComponentTransform* getTransform();
	float3 trans;
	float3 sca;
	Quat rot;
	float* matrix;
};