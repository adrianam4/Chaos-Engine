#pragma once

#include "Globals.h"
#include "Timer.h"
#include "GameTime.h"
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
	float lastFrameTime = 0.0f;

public:

	GameTime gameTime;
	float gameTimeNum = 0.0f;

	bool advance = true;

	bool playGameTime = false;
	bool pauseGameTime = false;
	bool stopGameTime = false;
	bool slowerGameTime = false;
	bool fasterGameTime = false;
	bool advanceGameTime = false;

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