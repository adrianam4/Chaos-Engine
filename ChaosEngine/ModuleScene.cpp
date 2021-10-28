#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include <string>

#include "assimp/cimport.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postProcess.h"

ModuleScene::ModuleScene(Application* app, bool startEnabled) : Module(app, startEnabled)
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

GameObject* ModuleScene::CreateGameObject(bool select, int number, const char* fmt, ...)
{
	GameObject* auxGameObject = new GameObject();
	std::string _name(fmt);
	std::string _num = std::to_string(number);
	auxGameObject->name = _name + _num;
	auxGameObject->selected = select;
	auxGameObject->id = lastId + 1;
	lastId++;

	return auxGameObject;
}