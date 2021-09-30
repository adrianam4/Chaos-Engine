#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	App->editor->AddLog("Loading Intro assets\n");
	bool ret = true;

	App->camera->Move(Vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(Vec3(0, 0, 0));
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	App->editor->AddLog("Unloading Intro scene\n");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	Plane plane(0, 0, 0, 0);
	plane.axis = true;
	plane.Render();

	Cube cube(1, 1, 1);
	cube.wire = true;
	cube.Render();

	return UPDATE_CONTINUE;
}