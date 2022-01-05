#include "Globals.h"
#include "Application.h"
#include "ModuleUI.h"
#include "CameraComponent.h"
#include <stack>

ModuleUI::ModuleUI(Application* app, bool startEnabled) : Module(app, startEnabled)
{
	focusedGameObject = nullptr;
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
	float2 mousePos = { (float)App->input->GetMouseX() ,(float)App->input->GetMouseY() };
	float2 mPos = { ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y };
	float4 viewport = App->editor->viewport;

	if (mousePos.x > viewport.x && mousePos.x < viewport.x + viewport.z && mousePos.y > viewport.y && mousePos.y < viewport.y + viewport.w)
	{
		float2 fMousePos = { mPos.x - viewport.x , mPos.y - viewport.y };
		float normalized_x = Lerp(-1, 1, fMousePos.x / viewport.z);
		float normalized_y = Lerp(1, -1, fMousePos.y / viewport.w);

		App->camera->editorCam->frustum.GetCornerPoints(&corners[0]);
		myRay = App->camera->editorCam->frustum.UnProjectLineSegment(normalized_x, normalized_y);

		for (int i = 0; i < App->scene->gameObjects.size(); i++)
		{
			GameObject* go = App->scene->gameObjects[i];
			for (int j = 0; j < go->boundingBoxes.size(); j++)
			{
				if (bool hit = myRay.Intersects(*go->aabb[j]))
				{
					if (go->SearchComponent(go, ComponentType::UI_BUTTON) != -1 || go->SearchComponent(go, ComponentType::UI_CHECKBOX) != -1 || go->SearchComponent(go, ComponentType::UI_IMAGE) != -1 ||
						go->SearchComponent(go, ComponentType::UI_INPUTBOX) != -1 || go->SearchComponent(go, ComponentType::UI_SLIDER) != -1)
					{
						hitObjs.push_back(go);
					}
				}
			}

		}

		if (hitObjs.size() > 0)
		{
			std::vector<float> distance;
			float nearDist = 500.0f;
			int nearObj = 0;
			for (int i = 0; i < hitObjs.size(); ++i)
			{
				int myComp = hitObjs[i]->SearchComponent(hitObjs[i], ComponentType::TRANSFORM2D);
				float3 distnceVec = hitObjs[i]->components[myComp]->position - App->camera->editorCam->frustum.pos;
				float finalDistance = math::Sqrt((distnceVec.x * distnceVec.x) + (distnceVec.y * distnceVec.y) + (distnceVec.z * distnceVec.z));
				distance.push_back(finalDistance);
			}
			for (int i = 0; i < distance.size(); i++)
			{
				if (distance[i] < nearDist)
				{
					nearDist = distance[i];
					nearObj = i;
				}
			}
			focusedGameObject = hitObjs[nearObj];
			hitObjs.clear();
		}
		else
			focusedGameObject = nullptr;
	}

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