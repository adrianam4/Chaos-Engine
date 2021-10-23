#pragma once
#include "Module.h"
#include "glmath.h"
#include <vector>
#include "GameObject.h"

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool startEnabled = true);
	~ModuleScene();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	
public:
	GameObject* CreateGameObject(const char* label, bool select);
	std::vector<GameObject*> gameObjects;
private:
	int lastId;
};