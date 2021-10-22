#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"

#include "assimp/cimport.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postProcess.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	lastId = -1;
}

ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init()
{
	bool ret = true;

	aiScene scene;

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

GameObject* ModuleScene::CreateGameObject(const char* label, bool select)
{
	GameObject* auxGameObject = new GameObject();

	auxGameObject->name = label;
	auxGameObject->selected = select;
	auxGameObject->id = lastId + 1;
	lastId++;

	return auxGameObject;
}