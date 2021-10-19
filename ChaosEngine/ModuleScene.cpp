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


	return UPDATE_CONTINUE;
}

update_status ModuleScene::Update(float dt)
{

	return UPDATE_CONTINUE;
}

update_status ModuleScene::PostUpdate(float dt)
{


	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{


	return true;
}

GameObject* ModuleScene::CreateGameObject()
{
	GameObject* auxGameObject = new GameObject();
	return auxGameObject;
}