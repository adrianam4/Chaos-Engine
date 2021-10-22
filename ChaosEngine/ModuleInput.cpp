#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput(Application* app, bool start_enabled) : Module(app, start_enabled)
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

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= SCREEN_SIZE;
	mouse_y /= SCREEN_SIZE;
	mouse_z = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if(mouse_buttons[i] == KEY_IDLE)
				mouse_buttons[i] = KEY_DOWN;
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if(mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_UP;
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;

	bool quit = false;
	while(SDL_PollEvent(&e))
	{
		ImGui_ImplSDL2_ProcessEvent(&e);
		switch(e.type)
		{
			case SDL_MOUSEWHEEL:
			mouse_z = e.wheel.y;
			break;

			case SDL_MOUSEMOTION:
			mouse_x = e.motion.x / SCREEN_SIZE;
			mouse_y = e.motion.y / SCREEN_SIZE;

			mouse_x_motion = e.motion.xrel / SCREEN_SIZE;
			mouse_y_motion = e.motion.yrel / SCREEN_SIZE;
			break;

			case SDL_DROPFILE:
			{
				char* fileDir = e.drop.file;
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
					App->scene->game_objects.push_back(App->scene->CreateGameObject(fileDir, false));
					int lastComponent = App->scene->game_objects.size() - 1;
					App->scene->game_objects[lastComponent]->components.push_back(App->scene->game_objects[lastComponent]->CreateComponent(ComponentType::MESH, fileDir));
					App->scene->game_objects[lastComponent]->components.push_back(App->scene->game_objects[lastComponent]->CreateComponent(ComponentType::TRANSFORM));
					App->scene->game_objects[lastComponent]->components[0]->owner = App->scene->game_objects[lastComponent];
					App->scene->game_objects[lastComponent]->components[1]->owner = App->scene->game_objects[lastComponent];
					SDL_free(&fileDir);
				}
				if ((aux[0] == 'p' && aux[1] == 'n' && aux[2] == 'g') || (aux[0] == 'd' && aux[1] == 'd' && aux[2] == 's'))
				{
					if (App->editor->objectSelected != nullptr)
					{
						App->editor->objectSelected->components.push_back(App->editor->objectSelected->CreateComponent(ComponentType::MATERIAL));
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

	if(quit == true || keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP)
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