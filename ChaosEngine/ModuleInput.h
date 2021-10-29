#pragma once
#include "Module.h"
#include "Globals.h"
#include "Mesh.h"
#include "Model.h"

#define MAX_MOUSE_BUTTONS 5

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ModuleInput : public Module
{
public:
	
	ModuleInput(Application* app, bool startEnabled = true);
	~ModuleInput();

	bool Init();
	update_status PreUpdate(float dt);
	bool CleanUp();

	KEY_STATE GetKey(int id) const
	{
		return keyboard[id];
	}

	KEY_STATE GetMouseButton(int id) const
	{
		return mouseButtons[id];
	}

	int GetMouseX() const
	{
		return mouseX;
	}

	int GetMouseY() const
	{
		return mouseY;
	}

	int GetMouseZ() const
	{
		return mouseZ;
	}

	int GetMouseXMotion() const
	{
		return mouseXMotion;
	}

	int GetMouseYMotion() const
	{
		return mouseYMotion;
	}

private:
	KEY_STATE mouseButtons[MAX_MOUSE_BUTTONS];
	int mouseX;
	int mouseY;
	int mouseZ;
	int mouseXMotion;
	int mouseYMotion;
public:
	SDL_Event e;
	const Uint8* keys;
	KEY_STATE* keyboard;
	char* fileDir = ".";
};