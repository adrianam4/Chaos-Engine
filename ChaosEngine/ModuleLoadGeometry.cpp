#include "Globals.h"
#include "Application.h"
#include "ModuleLoadGeometry.h"

#include "Assimp/include/assimp/cimport.h"
#include "Assimp/include/assimp/scene.h"
#include "Assimp/include/assimp/postprocess.h"
#pragma comment (lib, "Assimp/x86-Release/assimp-vc142-mt.lib")

ModuleLoadGeometry::ModuleLoadGeometry(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleLoadGeometry::~ModuleLoadGeometry()
{}

// -----------------------------------------------------------------
bool ModuleLoadGeometry::Start()
{
	App->editor->AddLog("Loading geometry\n");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
update_status ModuleLoadGeometry::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
update_status ModuleLoadGeometry::Update(float dt)
{
	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
update_status ModuleLoadGeometry::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
bool ModuleLoadGeometry::CleanUp()
{
	App->editor->AddLog("Unloading geometry\n");

	return true;
}