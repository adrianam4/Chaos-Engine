#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

#include "MathGeoLib/src/MathGeoLib.h"

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

	void CalculateViewMatrix();

public:

	Vec3 x, y, z, position, reference;
	Frustum camFrustum;
	float aspectRatio = 1.f;
	float verticalFOV = 60.f;
	float nearPlaneDistance = 0.1f;
	float farPlaneDistance = 5000.f;

private:

	mat4x4 viewMatrix, viewMatrixInverse;
};