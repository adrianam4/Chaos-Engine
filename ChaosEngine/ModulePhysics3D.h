#pragma once
#include "Module.h"
#include "Globals.h"

// TODO 1: Add Bullet common include btBulletDynamicsCommon.h

class DebugDrawer;

class ModulePhysics3D : public Module
{
public:
	ModulePhysics3D(Application* app, bool start_enabled = true);
	~ModulePhysics3D();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

private:

	bool debug;

	DebugDrawer* debug_draw;
};