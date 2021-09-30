#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const Vec3 &Position, const Vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const Vec3 &Spot);
	void Move(const Vec3 &Movement);
	float* GetViewMatrix();

private:

	void CalculateViewMatrix();

public:
	
	Vec3 X, Y, Z, Position, Reference;

private:

	mat4x4 ViewMatrix, ViewMatrixInverse;
};