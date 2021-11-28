#pragma once

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "ModuleViewportFrameBuffer.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "ModuleResources.h"
#include "FileSystem.h"

#include <iostream>
#include <list>
#include <vector>


class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleScene* scene;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleEditor* editor;
	ModuleResources* resources;
	ModuleViewportFrameBuffer* viewportBuffer;
	FileSystem* fileSystem;

private:

	Timer	msTimer;
	float	dt;
	std::list<Module*> listModules;
	float mLastFrameTime = 0.0f;
public:
	int maxMs;

	bool gameMode = false;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

extern Application* App;