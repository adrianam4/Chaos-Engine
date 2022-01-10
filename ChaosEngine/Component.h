#pragma once
#include "Globals.h"
#include "Module.h"
#include "glmath.h"
#include <vector>
#include "MathGeoLib.h"

class GameObject;

enum class ComponentType
{
	TRANSFORM2D,
	TRANSFORM = 1,
	MESH,
	CUBE,
	PYRAMID,
	SPHERE,
	CYLINDER,
	PLANE,
	MATERIAL,
	CAMERA,
	UI_BUTTON,
	UI_IMAGE,
	UI_CHECKBOX,
	UI_SLIDER,
	UI_INPUTBOX,
	NONE,
	EMPTY,
};

enum class State
{
	DISABLED,
	NORMAL,
	FOCUSED,
	PRESSED,
	SELECTED
};

class Component
{
public:
	virtual ~Component();
	ComponentType type;
	bool active;
	//ESTO QUE HACE AQUI ?? 
	bool drawNormals = false;
	bool drawWireframe = false;
	GameObject* owner;
	const char* name;
	std::string modelPath;
	const char* texturePath;
	int width;
	int height;
	u32 UID;
	
	float3 lastRotation;
	float3 lastPosition;
	float3 lastScale;
	float3 position;
	float lastGeneralScale;

	virtual void CreateAABB(ComponentType type, GameObject* go, bool firstTime){}
	float3 scale;
	float3 rotationEuler;
	float generalScale;
	float4x4 transmat;
	Quat rotationQuat;
	
	float horizontalFov;
	float nearPlaneDistance;
	float farPlaneDistance;
	// ESTO DEBERIA ESTAR SOLO EN LOS COMPONENTES CONCRETOS
	virtual void setOwner(){}
	virtual void Enable() {}
	virtual void Update() {}
	virtual void Update(bool releaseMouse){}
	virtual void Disable() {}
	virtual void Draw() {}
	virtual void OnClick() {}
	virtual void Save(const char* texturePath) {}
	virtual void Load(const char* texturePath) {}
	virtual void OnEditor(int i) {}
	u32 GenerateUID();
};