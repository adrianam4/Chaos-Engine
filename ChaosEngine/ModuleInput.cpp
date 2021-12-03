#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include"Importer.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput(Application* app, bool startEnabled) : Module(app, startEnabled)
{
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init()
{
	App->editor->AddLog("Init SDL input event system\n");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		App->editor->AddLog("SDL_EVENTS could not initialize! SDL_Error: %s\n");
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{
	SDL_PumpEvents();

	keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouseX, &mouseY);

	mouseX /= SCREEN_SIZE;
	mouseY /= SCREEN_SIZE;
	mouseZ = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if(mouseButtons[i] == KEY_IDLE)
				mouseButtons[i] = KEY_DOWN;
			else
				mouseButtons[i] = KEY_REPEAT;
		}
		else
		{
			if(mouseButtons[i] == KEY_REPEAT || mouseButtons[i] == KEY_DOWN)
				mouseButtons[i] = KEY_UP;
			else
				mouseButtons[i] = KEY_IDLE;
		}
	}

	mouseXMotion = mouseYMotion = 0;

	bool quit = false;
	while(SDL_PollEvent(&e))
	{
		ImGui_ImplSDL2_ProcessEvent(&e);
		switch(e.type)
		{
			case SDL_MOUSEWHEEL:
			mouseZ = e.wheel.y;
			break;

			case SDL_MOUSEMOTION:
			mouseX = e.motion.x / SCREEN_SIZE;
			mouseY = e.motion.y / SCREEN_SIZE;

			mouseXMotion = e.motion.xrel / SCREEN_SIZE;
			mouseYMotion = e.motion.yrel / SCREEN_SIZE;
			break;

			case SDL_DROPFILE:
			{
				fileDir = e.drop.file;
				char aux[3];
				int lastChar = strlen(fileDir) - 1;
				int j = 2;
				for (int i = lastChar; i > lastChar - 3; i--)
				{
					aux[j] = fileDir[i];
					j--;
				}
				if ((aux[0] == 'f' && aux[1] == 'b' && aux[2] == 'x') || (aux[0] == 'o' && aux[1] == 'b' && aux[2] == 'j') || (aux[0] == 'F' && aux[1] == 'B' && aux[2] == 'X'))
				{

					FBXimporter importer;
					importer.SpecialreadFromFBX(fileDir);

					/*static uint gObjs = 1;
					App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, gObjs, "Game Object"));
					gObjs++;
					GameObject* gObj = App->scene->gameObjects[App->scene->gameObjects.size() - 1];
					App->editor->objectSelected = gObj;
					
					model = App->resources->ImportFile((const char*)fileDir);
					App->resources->LoadResource(model);
					gObj->components[0]->owner = gObj;
					gObj->components.push_back(gObj->CreateComponent(ComponentType::TRANSFORM, &float3(0, 0, 0), &float3(1, 1, 1), &float3(0, 0, 0)));
					gObj->components[1]->owner = gObj;
					tex = App->resources->ImportFile("Assets/Textures/Checker.png");
					App->resources->LoadResource(tex);
					gObj->components[2]->owner = gObj;
					gObj->components.erase(gObj->components.begin() + 2);*/

					SDL_free(&fileDir);
					App->editor->AddLog("Loaded %s\n", fileDir);
				}
				if ((aux[0] == 'p' && aux[1] == 'n' && aux[2] == 'g') || (aux[0] == 'd' && aux[1] == 'd' && aux[2] == 's') || (aux[0] == 'P' && aux[1] == 'N' && aux[2] == 'G') || (aux[0] == 'D' && aux[1] == 'D' && aux[2] == 'S'))
				{
					if (App->editor->objectSelected != nullptr)
					{
						int oldMaterialId;
						oldMaterialId = App->editor->objectSelected->SearchComponent(App->editor->objectSelected, ComponentType::MATERIAL);
						if (oldMaterialId != -1) 
						{
							App->editor->objectSelected->components.erase(App->editor->objectSelected->components.begin() + oldMaterialId);
						}
						u32 textId = App->resources->ImportFile(fileDir);
						App->resources->LoadResource(textId);
						App->editor->objectSelected->components[App->editor->objectSelected->components.size() - 1]->owner = App->editor->objectSelected;
						SDL_free(&fileDir);
					}
				}
			}
			break;

			case SDL_QUIT:
			quit = true;
			break;

			case SDL_WINDOWEVENT:
			{
				if(e.window.event == SDL_WINDOWEVENT_RESIZED)
					App->renderer3D->OnResize(e.window.data1, e.window.data2);
			}
		}
	}

	if (quit == true || keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	App->editor->AddLog("Quitting SDL input event subsystem\n");

	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}