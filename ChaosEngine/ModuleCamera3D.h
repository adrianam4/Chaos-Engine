#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

#include "MathGeoLib/src/MathGeoLib.h"

class ComponentCamera;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool startEnabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const Vec3& position, const Vec3& reference, bool rotateAroundReference = false);
	void LookAt(const Vec3& spot);
	void Move(const Vec3& movement);
	float* GetViewMatrix();
	void RecalculateProjection();
private:

	void CalculateViewMatrix(ComponentCamera* cam);

public:
	LineSegment myRay;
	float3 corners[8];
	//Frustum frustum;
	ComponentCamera* GameCam=nullptr;
	ComponentCamera *cam;
	float aspectRatio = 16/9;
	float verticalFOV = 75.0f;
	float nearPlaneDistance = 1.0f;
	float farPlaneDistance = 20.0f;
	
	std::vector<ComponentCamera*>camArray;
private:
	mat4x4 viewMatrix, viewMatrixInverse;
	std::vector<GameObject*> hitObjs;
};