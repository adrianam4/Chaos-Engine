#include "Globals.h"
#include "Application.h"
#include "ModuleUI.h"

ModuleUI::ModuleUI(Application* app, bool startEnabled) : Module(app, startEnabled)
{
}

ModuleUI::~ModuleUI()
{
}

bool ModuleUI::Init()
{
	bool ret = true;

	return ret;
}

update_status ModuleUI::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleUI::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleUI::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleUI::CleanUp()
{
	return true;
}