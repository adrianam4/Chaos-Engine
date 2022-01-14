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
#include "Transform2DComponent.h"
#include "Resource.h"

class theBuffer;
class ButtonComponent;
class ImageComponent;
class CheckboxComponent;
class InputBoxComponent;
class SliderComponent;
class ComponentMaterial;

class GameObject
{
public:
	virtual ~GameObject();
	void Update();
	Component* CreateComponent2(ComponentType type, float3 pos, double hFov, double nPlane, double fPlane, bool isObj);
	Component* CreateUIComponent(ComponentType type, std::string text);
	Component* CreateComponent(ComponentType type, float3* pos = nullptr, float3* measures = nullptr, float3* rotation = nullptr);
	Component* CreateComponent(ComponentType type, const char* name, bool isDropped);
	int SearchComponent(GameObject* gameObject, ComponentType type);
	Component*  CreateOneMeshComponent(theBuffer* temporal, const char* name);
	u32 GenerateUID();
	Component* CreateMeshComponent(std::vector<theBuffer*>* theArray,const char* path);
	Component* CreateComponentWithResource(Resource* meshResource);
public:
	std::vector<float3> GetVertices(int id);
	std::vector<float3> GetNormals(int id);
	std::vector<float2> GetTexCoords(int id);
	std::vector<uint> GetIndices(int id);
	bool isImported = false;
	u32 UID;
	u32 parentUID;
	uint id;
	bool active;
	bool selected;
	bool isStatic = false;
	std::string name;
	std::vector<Component*> components;
	std::vector<GameObject*> childrens;
	std::vector<BoundingBoxes*> boundingBoxes;
	GameObject* parent;
	std::vector<AABB*>aabb;
	std::vector<OBB*>obb;
	bool isChild = false;
	ComponentTransform* getTransform();
	ComponentTransform2D* getTransform2D();
	ButtonComponent* GetButtonComponent(GameObject* go);
	ImageComponent* GetImageComponent(GameObject* go);
	CheckboxComponent* GetCheckboxComponent(GameObject* go);
	InputBoxComponent* GetInputboxComponent(GameObject* go);
	SliderComponent* GetSliderComponent(GameObject* go);
	ComponentMaterial* GetMaterialComp(GameObject* go);
	float3 trans;
	float3 sca;
	Quat rot;
	float* matrix;
	theBuffer buffer;
	
};