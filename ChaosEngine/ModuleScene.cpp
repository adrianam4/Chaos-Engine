#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init()
{
	bool ret = true;


	return ret;
}

update_status ModuleScene::PreUpdate(float dt)
{
	update_status status = update_status::UPDATE_CONTINUE;


	return status;
}

update_status ModuleScene::Update(float dt)
{
	update_status status = update_status::UPDATE_CONTINUE;


	return status;
}

update_status ModuleScene::PostUpdate(float dt)
{
	update_status status = update_status::UPDATE_CONTINUE;


	return status;
}

bool ModuleScene::CleanUp()
{
	bool ret = true;


	return ret;
}

GameObject* ModuleScene::CreateGameObject()
{
	return nullptr;
}