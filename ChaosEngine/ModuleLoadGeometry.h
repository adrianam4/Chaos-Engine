#pragma once
#include "Module.h"
#include "Globals.h"

class ModuleLoadGeometry : public Module
{
public:
	ModuleLoadGeometry(Application* app, bool start_enabled = true);
	~ModuleLoadGeometry();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
};