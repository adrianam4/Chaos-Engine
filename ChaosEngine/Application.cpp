#include "Application.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	scene = new ModuleScene(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	editor = new ModuleEditor(this);
	resources = new ModuleResources(this);
	viewportBuffer = new ModuleViewportFrameBuffer(this);
	fileSystem = new FileSystem(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(scene);
	AddModule(editor);
	AddModule(resources);
	AddModule(viewportBuffer);
	AddModule(fileSystem);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	std::list<Module*>::iterator item = --listModules.end();

	//TODO MEMORY LEAK
	while(item != listModules.begin())
	{
		delete (*item);
		item--;
	}
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	std::list<Module*>::iterator item = listModules.begin();

	while(item != listModules.end() && ret == true)
	{
		ret = (*item)->Init();
		*item++;
	}

	// After all Init calls we call Start() in all modules
	
	item = listModules.begin();

	while(item != listModules.end() && ret == true)
	{
		ret = (*item)->Start();
		*item++;
	}
	
	msTimer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)msTimer.Read() / 1000.0f;
	msTimer.Start();

	if (advance = false)
	{
		gameTime.IsPaused();
		advance = true;
	}

	if (playGameTime)
	{
		gameTime.Start();
	}

	if (stopGameTime)
		gameTime.Stop();

	if (pauseGameTime)
	{
		if (gameTime.IsPaused())
			gameTime.Play();
		else
			gameTime.Pause();
	}

	if (advanceGameTime) 
	{
		if (gameTime.IsPaused())
		{
			if (advance)
			{
				gameTime.Play();
				advance = false;
			}
		}
	}

	if (fasterGameTime)
	{
		msTimer * 2;
	}

	if (slowerGameTime)
	{
		msTimer / 2;
	}

	playGameTime = false;
	pauseGameTime = false;
	stopGameTime = false;
	slowerGameTime = false;
	fasterGameTime = false;
	advanceGameTime = false;

	gameTimeNum = gameTime.ReadSec();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	unsigned int lastFrameMs = msTimer.Read();

	if ((maxMs > 0) && (lastFrameMs < maxMs))
	{
		SDL_Delay(maxMs - lastFrameMs);
	}
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	float time = (float)SDL_GetPerformanceCounter();
	Timer timestep = time - lastFrameTime;
	lastFrameTime = time;

	//App->editor->AddLog("Delta time: %fs (%fms)\n", timestep.GetSeconds(), timestep.GetMilliseconds());

	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	std::list<Module*>::iterator item = listModules.begin();

	
	while(item != listModules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PreUpdate(dt);
		*item++;
	}

	item = listModules.begin();

	while(item != listModules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->Update(dt);
		*item++;
	}

	item = listModules.begin();

	while(item != listModules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PostUpdate(dt);
		*item++;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	std::list<Module*>::iterator item = listModules.end();

	while(item != listModules.end() && ret == true)
	{
		ret = (*item)->CleanUp();
		*item++;
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	listModules.push_back(mod);
}