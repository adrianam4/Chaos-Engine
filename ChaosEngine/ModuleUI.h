#pragma once
#include "Module.h"

class ModuleUI : public Module
{
public:
	ModuleUI(Application* app, bool startEnabled = true);
	~ModuleUI();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

public:
	LineSegment myRay;
	float3 corners[8];
	std::vector<GameObject*> hitObjs;
	GameObject* focusedGameObject;
};