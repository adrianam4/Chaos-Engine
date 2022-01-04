#include "Globals.h"
#include "Application.h"
#include "ModuleUI.h"

ModuleUI::ModuleUI(Application* app, bool startEnabled) : Module(app, startEnabled)
{
}

ModuleUI::~ModuleUI()
{
}

bool ModuleUI::Init()
{
	bool ret = true;

	return ret;
}

update_status ModuleUI::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleUI::Update(float dt)
{
	// Update All UI Components
	if (App->gameMode)
	{
		for (int i = 0; i < App->scene->gameObjects.size(); i++)
		{
			GameObject* go = App->scene->gameObjects[i];

			int button = go->SearchComponent(go, ComponentType::UI_BUTTON);
			int checkbox = go->SearchComponent(go, ComponentType::UI_CHECKBOX);
			int image = go->SearchComponent(go, ComponentType::UI_IMAGE);
			int inputbox = go->SearchComponent(go, ComponentType::UI_INPUTBOX);
			int slider = go->SearchComponent(go, ComponentType::UI_SLIDER);

			if (button != -1)
				go->components[button]->Update();
			if (checkbox != -1)
				go->components[checkbox]->Update();
			if (image != -1)
				go->components[image]->Update();
			if (inputbox != -1)
				go->components[inputbox]->Update();
			if (slider != -1)
				go->components[slider]->Update();
		}
	}
	
	return UPDATE_CONTINUE;
}

update_status ModuleUI::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleUI::CleanUp()
{
	return true;
}