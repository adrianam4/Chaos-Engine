#include "Application.h"
#include "Globals.h"
#include "ModuleEditor.h"
#include "ModuleScene.h"
#include "Mesh.h"

#include "shellapi.h"
#include <GL/GL.h>
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "Parson/parson.h"

#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

#define DIV 1048576

ModuleEditor::ModuleEditor(Application* app, bool startEnabled) : Module(app, startEnabled)
{
	lastId = -1;
	objectSelected = nullptr;
	consoleBuffer.clear();
}

ModuleEditor::~ModuleEditor()
{
	objectSelected = nullptr;
	consoleBuffer.clear();
}

// Load assets
bool ModuleEditor::Start()
{
	App->editor->AddLog("Loading Editor Assets\n");
	bool ret = true;

	grid = new Grid(0, 0, 0, 0);
	grid->axis = true;

	App->camera->Move(Vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(Vec3(0, 0, 0));
	
	LoadConfig();
	ComproveScreen();

	App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, 1, "House "));
	int lastComponent = App->scene->gameObjects.size() - 1;
	App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::MESH, "Assets/Models/BakerHouse.fbx",false));
	App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::TRANSFORM));
	App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::MATERIAL, "Assets/Textures/BakerHouse.png",false));
	App->scene->gameObjects[lastComponent]->components[0]->owner = App->scene->gameObjects[lastComponent];
	App->scene->gameObjects[lastComponent]->components[1]->owner = App->scene->gameObjects[lastComponent];
	App->scene->gameObjects[lastComponent]->components[2]->owner = App->scene->gameObjects[lastComponent];
	SDL_free("Assets/Textures/BakerHouse.fbx");

	return ret;
}

// Load assets
bool ModuleEditor::CleanUp()
{
	App->editor->AddLog("Unloading Editor scene\n");
	consoleBuffer.clear();

	for (int i = 0; i < cubes.size(); i++)
	{
		delete cubes[i];
	}

	for (int i = 0; i < pyramids.size(); i++)
	{
		delete pyramids[i];
	}

	for (int i = 0; i < spheres.size(); i++)
	{
		delete spheres[i];
	}

	for (int i = 0; i < cylinders.size(); i++)
	{
		delete cylinders[i];
	}

	return true;
}

void ModuleEditor::SaveConfig()
{
	//Creating Json file
	JSON_Value* user_data = json_value_init_object();

	json_object_set_number(json_object(user_data), "MaxFPS", maxFPS);
	json_object_set_number(json_object(user_data), "MaxMs", App->maxMs);
	json_object_set_number(json_object(user_data), "Width", width);
	json_object_set_number(json_object(user_data), "Height", height);
	json_object_set_number(json_object(user_data), "Brightness", brightness);
	json_object_set_boolean(json_object(user_data), "Fullscreen", fullscreen);
	json_object_set_boolean(json_object(user_data), "Resizable", resizable);
	json_object_set_boolean(json_object(user_data), "Borderless", borderless);
	json_object_set_boolean(json_object(user_data), "Dekstop", dekstop);
	json_object_set_boolean(json_object(user_data), "AABB", showAABB);

	json_serialize_to_file_pretty(user_data, "ConfigFile.json");

	AddLog("Saved Config Data\n");
}

void ModuleEditor::LoadConfig()
{
	//Reading JSON File
	JSON_Value* userData = json_parse_file("ConfigFile.json");

	maxFPS = json_object_get_number(json_object(userData), "MaxFPS");
	App->maxMs = json_object_get_number(json_object(userData), "MaxMs");
	width = json_object_get_number(json_object(userData), "Width");
	height = json_object_get_number(json_object(userData), "Height");
	brightness = json_object_get_number(json_object(userData), "Brightness");
	fullscreen = json_object_get_boolean(json_object(userData), "Fullscreen");
	resizable = json_object_get_boolean(json_object(userData), "Resizable");
	borderless = json_object_get_boolean(json_object(userData), "Borderless");
	dekstop = json_object_get_boolean(json_object(userData), "Dekstop");
	showAABB = json_object_get_boolean(json_object(userData), "AABB");

	AddLog("Loaded Config Data\n");
}

void ModuleEditor::ComproveScreen()
{
	if (fullscreen)
		App->window->SetFullscreen(fullscreen);
	else if (resizable)
		App->window->SetResizable(resizable);
	else if (borderless)
		App->window->SetBorderless(borderless);
	else if (dekstop)
		App->window->SetDekstop(dekstop);
}

void ModuleEditor::AddLog(const char* fmt, ...)
{
		LOGCE(fmt);
		va_list args;
		va_start(args, fmt);
		consoleBuffer.appendfv(fmt, args);
		va_end(args);
		scrollToBottom = true;
}

void ModuleEditor::AddCube(float3 pos, float3 sca)
{
	MyCube *auxCube;
	auxCube = new MyCube(pos,sca);
	cubes.push_back(auxCube);
}

void ModuleEditor::AddPyramid(float3 pos, float3 sca)
{
	MyPyramid* auxPyramid;
	auxPyramid = new MyPyramid(pos, sca);
	pyramids.push_back(auxPyramid);
}

void ModuleEditor::AddSphere(float3 pos, float3 sca)
{
	MySphere* auxSphere;
	auxSphere = new MySphere(1, 20, 20, pos, sca);
	spheres.push_back(auxSphere);
}
void ModuleEditor::AddPlane(float3 pos, float3 sca)
{
	MyPlane* auxPlane;
	auxPlane = new MyPlane(pos, sca);
	planes.push_back(auxPlane);
}
void ModuleEditor::AddCylinder(float3 pos, float3 sca)
{
	MyCylinder* auxCylinder;
	auxCylinder = new MyCylinder(pos,sca);
	cylinders.push_back(auxCylinder);
}
void ModuleEditor::DOptionsmenu(ComponentType type) {
	
	switch (type)
	{
	case ComponentType::CUBE:

		ImGui::CloseCurrentPopup();

		ImGui::Begin("Options");
		ImGui::Text("Set Position:");
		ImGui::DragFloat("X Position", &position.x);
		ImGui::DragFloat("Y Position", &position.y);
		ImGui::DragFloat("Z Position", &position.z);
		ImGui::Text("Set Scale:");
		ImGui::DragFloat("X Scale", &M.x);
		ImGui::DragFloat("Y Scale", &M.y);
		ImGui::DragFloat("Z Scale", &M.z);

		if (ImGui::Button("Create Cube")) {
			if (cubes.size() == 0)
				App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, 1, "Cube "));
			else
				App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, cubes.size() + 1, "Cube "));

			int lastComponent = App->scene->gameObjects.size() - 1;
			App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::CUBE, &position,&M));
			App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::TRANSFORM));
			App->scene->gameObjects[lastComponent]->components[0]->owner = App->scene->gameObjects[lastComponent];
			App->scene->gameObjects[lastComponent]->components[1]->owner = App->scene->gameObjects[lastComponent];
			showOptionsMenu = ComponentType::NONE;
			App->editor->AddLog("Cube Created\n");
		}
		ImGui::End();
		break;
	
	case ComponentType::PYRAMID:

		ImGui::Begin("Options");

		ImGui::Text("Set Position:");
		ImGui::DragFloat("X Position", &position.x);
		ImGui::DragFloat("Y Position", &position.y);
		ImGui::DragFloat("Z Position", &position.z);

		ImGui::Text("Set Scale:");
		ImGui::DragFloat("X Scale", &M.x);
		ImGui::DragFloat("Y Scale", &M.y);
		ImGui::DragFloat("Z Scale", &M.z);

		if (ImGui::Button("Create Pyramid")) 
		{
			if (pyramids.size() == 0)
				App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, 1, "Pyramid "));
			else
				App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, pyramids.size() + 1, "Pyramid "));

			int lastComponent = App->scene->gameObjects.size() - 1;
			App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::PYRAMID, &position,&M));
			App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::TRANSFORM));
			App->scene->gameObjects[lastComponent]->components[0]->owner = App->scene->gameObjects[lastComponent];
			App->scene->gameObjects[lastComponent]->components[1]->owner = App->scene->gameObjects[lastComponent];
			showOptionsMenu = ComponentType::NONE;
			App->editor->AddLog("Pyramid Created\n");
		}
		ImGui::End();
		break;
	case ComponentType::CYLINDER:
		ImGui::Begin("Options");

		ImGui::Text("Set Position:");
		ImGui::DragFloat("X Position", &position.x);
		ImGui::DragFloat("Y Position", &position.y);
		ImGui::DragFloat("Z Position", &position.z);

		//ImGui::Text("Set Scale:");
		//ImGui::DragFloat("X Scale", &M.x);
		//ImGui::DragFloat("Y Scale", &M.y);
		//ImGui::DragFloat("Z Scale", &M.z);

		if (ImGui::Button("Create Cylindrer")) 
		{
			if (cylinders.size() == 0)
				App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, 1, "Cylinder "));
			else
				App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, cylinders.size() + 1, "Cylinder "));

			int lastComponent = App->scene->gameObjects.size() - 1;
			App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::CYLINDER, &position, &M));
			App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::TRANSFORM));
			App->scene->gameObjects[lastComponent]->components[0]->owner = App->scene->gameObjects[lastComponent];
			App->scene->gameObjects[lastComponent]->components[1]->owner = App->scene->gameObjects[lastComponent];
			showOptionsMenu = ComponentType::NONE;
			App->editor->AddLog("Cylinder Created\n");
		}
		ImGui::End();
		break;
	case ComponentType::SPHERE:

		ImGui::Begin("Options");

		ImGui::Text("Set Position:");
		ImGui::DragFloat("X Position", &position.x);
		ImGui::DragFloat("Y Position", &position.y);
		ImGui::DragFloat("Z Position", &position.z);

		//ImGui::Text("Set Scale:");
		//ImGui::DragFloat("X Scale", &M.x);
		//ImGui::DragFloat("Y Scale", &M.y);
		//ImGui::DragFloat("Z Scale", &M.z);

		if (ImGui::Button("Create Sphere")) 
		{
			if (spheres.size() == 0)
				App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, 1, "Sphere "));
			else
				App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, spheres.size() + 1, "Sphere "));

			int lastComponent = App->scene->gameObjects.size() - 1;
			App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::SPHERE, &position, &M));
			App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::TRANSFORM));
			App->scene->gameObjects[lastComponent]->components[0]->owner = App->scene->gameObjects[lastComponent];
			App->scene->gameObjects[lastComponent]->components[1]->owner = App->scene->gameObjects[lastComponent];
			showOptionsMenu = ComponentType::NONE;
			App->editor->AddLog("Sphere Created\n");
		}
		ImGui::End();
		break;
		
	case ComponentType::PLANE:

		ImGui::Begin("Options");

		ImGui::Text("Set Position:");
		ImGui::DragFloat("X Position", &position.x);
		ImGui::DragFloat("Y Position", &position.y);
		ImGui::DragFloat("Z Position", &position.z);

		ImGui::Text("Set Scale:");
		ImGui::DragFloat("X Scale", &M.x);
		ImGui::DragFloat("Y Scale", &M.y);
		ImGui::DragFloat("Z Scale", &M.z);

		if (ImGui::Button("Create Plane"))
		{
			if (planes.size() == 0)
				App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, 1, "Plane "));
			else
				App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, planes.size() + 1, "Plane "));

			int lastComponent = App->scene->gameObjects.size() - 1;
			App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::PLANE, &position, &M));
			App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::TRANSFORM));
			App->scene->gameObjects[lastComponent]->components[0]->owner = App->scene->gameObjects[lastComponent];
			App->scene->gameObjects[lastComponent]->components[1]->owner = App->scene->gameObjects[lastComponent];
			showOptionsMenu = ComponentType::NONE;
			App->editor->AddLog("Plane Created\n");
		}
		ImGui::End();
		break;
	default:
		break;
	}
}
// Update: draw background
update_status ModuleEditor::Update(float dt)
{
	if (objectSelected != nullptr && App->input->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN)
	{
		int i;
		int id = objectSelected->id;
		for (i = 0; i < App->scene->gameObjects.size(); i++)
		{
			if (id == App->scene->gameObjects[i]->id)
			{
				objectSelected = nullptr;
				
				for (int b = 0; b < App->scene->gameObjects.size(); b++)
				{
					for (int a = 0; a < App->scene->gameObjects[b]->childrens.size(); a++)
					{
						if (App->scene->gameObjects[i] == App->scene->gameObjects[b]->childrens[a])
						{
							App->scene->gameObjects[b]->childrens.erase(App->scene->gameObjects[b]->childrens.begin() + a);
							//App->scene->gameObjects.erase(App->scene->gameObjects[b]->childrens.begin() + a);
						}

					}
					//App->scene->gameObjects.erase(App->scene->gameObjects[i]->childrens.begin()+a);
				}

				for (int a = 0; a < App->scene->gameObjects[i]->childrens.size(); a++) 
				{
					for (int b = 0; b < App->scene->gameObjects.size(); b++)
					{
						if (App->scene->gameObjects[i]->childrens[a] == App->scene->gameObjects[b]) 
						{
							App->scene->gameObjects.erase(App->scene->gameObjects.begin() + b);
						}
						
					}
					//App->scene->gameObjects.erase(App->scene->gameObjects[i]->childrens.begin()+a);
					
				}
				App->scene->gameObjects.erase(App->scene->gameObjects.begin() + i);
				
				break;
			}
			if (App->editor->objectSelected->components[i]->type == ComponentType::CUBE)
			{
				App->editor->AddLog("Cube Deleted\n");
			}
			if (App->editor->objectSelected->components[i]->type == ComponentType::PYRAMID)
			{
				App->editor->AddLog("Pyramid Deleted\n");
			}
			if (App->editor->objectSelected->components[i]->type == ComponentType::CYLINDER)
			{
				App->editor->AddLog("Cylinder Deleted\n");
			}
			if (App->editor->objectSelected->components[i]->type == ComponentType::SPHERE)
			{
				App->editor->AddLog("Sphere Deleted\n");
			}
			if (App->editor->objectSelected->components[i]->type == ComponentType::PLANE)
			{
				App->editor->AddLog("Plane Deleted\n");
			}
			if (App->editor->objectSelected->components[i]->type == ComponentType::MESH)
			{
				App->editor->AddLog("Mesh Deleted\n");
			}
			if (App->editor->objectSelected->components[i]->type == ComponentType::EMPTY)
			{
				App->editor->AddLog("Empty Object Deleted\n");
			}
		}
	}
	// Our state
	static bool showDemoWindow = false;
	static bool showAnotherWindow = false;
	static bool showCloseWindow = true;
	static bool showAboutWindow = false;
	static bool openConfigMenu = false;
	static bool showConfigMenu = true;
	static bool showHierarchy = true;
	static bool showInspector = true;
	static bool showConsoleMenu = true;
	static bool showOptions = true;
	static bool isActive = true;
	static bool isActive2 = true;
	static bool isActive3 = true;
	static bool isActive4 = true;
	ImVec4 clearColor = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		SaveConfig();

	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		LoadConfig();
		ComproveScreen();
	}
	//////////////////////////////////////////////////////////////////////////////////////////// SHAPES BEGIN OPTIONS ////////////////////////////////////////////////////////////////////////////////////////////
	if (showOptionsMenu != ComponentType::NONE) {
		DOptionsmenu(showOptionsMenu);

	}
	//////////////////////////////////////////////////////////////////////////////////////////// MAIN MENU BAR ////////////////////////////////////////////////////////////////////////////////////////////

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File", &openConfigMenu))
		{
			if (ImGui::MenuItem("Quit"))
			{
				return UPDATE_STOP;
			}
			if (ImGui::MenuItem("Save"))
			{
				SaveConfig();
			}
			if (ImGui::MenuItem("Load"))
			{
				LoadConfig();
				ComproveScreen();
			}
			ImGui::EndMenu();
		}


		if (ImGui::BeginMenu("View", &openConfigMenu))
		{
			if (ImGui::MenuItem("Configuration"))
			{
				showConfigMenu = !showConfigMenu;
			}
			if (ImGui::MenuItem("Hierarchy"))
			{
				showHierarchy = !showHierarchy;
			}
			if (ImGui::MenuItem("Inspector"))
			{
				showInspector = !showInspector;
			}
			if (ImGui::MenuItem("Console"))
			{
				showConsoleMenu = !showConsoleMenu;
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Enable/Disable AABB"))
			{
				showAABB = !showAABB;
				if (showAABB)
				{
					App->editor->AddLog("AABB Enabled\n");
				}
				else
				{
					App->editor->AddLog("AABB Disabled\n");
				}
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("GameObject", &openConfigMenu))
		{
			if (ImGui::MenuItem("Create Empty"))
			{
				static int empties = 1;
				App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, empties, "Empty "));
				empties++;
				int lastComponent = App->scene->gameObjects.size() - 1;
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::EMPTY, &position));
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::TRANSFORM));
				App->scene->gameObjects[lastComponent]->components[0]->owner = App->scene->gameObjects[lastComponent];
				App->scene->gameObjects[lastComponent]->components[1]->owner = App->scene->gameObjects[lastComponent];
				App->editor->AddLog("Empty Object Created\n");
			}
			if (ImGui::MenuItem("Create House"))
			{
				static int houses = 2;
				App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, houses, "House "));
				houses++;
				int lastComponent = App->scene->gameObjects.size() - 1;
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::MESH, "Assets/Models/BakerHouse.fbx",false));
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::TRANSFORM));
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::MATERIAL, "Assets/Textures/BakerHouse.png", false));
				App->scene->gameObjects[lastComponent]->components[0]->owner = App->scene->gameObjects[lastComponent];
				App->scene->gameObjects[lastComponent]->components[1]->owner = App->scene->gameObjects[lastComponent];
				App->editor->AddLog("House Created\n");
			}
			if (ImGui::MenuItem("Create Penguin"))
			{
				static int penguins = 1;
				App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, penguins, "Penguin "));
				penguins++;
				int lastComponent = App->scene->gameObjects.size() - 1;
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::MESH, "Assets/Models/Penguin.fbx", false));
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::TRANSFORM));
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::MATERIAL, "Assets/Textures/Penguin.png", false));
				App->scene->gameObjects[lastComponent]->components[0]->owner = App->scene->gameObjects[lastComponent];
				App->scene->gameObjects[lastComponent]->components[1]->owner = App->scene->gameObjects[lastComponent];
				App->editor->AddLog("Penguin Created\n");
			}
			if (ImGui::MenuItem("Create Car"))
			{
				static int cars = 1;
				App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, cars, "Car "));
				cars++;
				int lastComponent = App->scene->gameObjects.size() - 1;
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::MESH, "Assets/Models/Car.fbx", false));
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::TRANSFORM));
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::MATERIAL, "Assets/Textures/Car.png", false));
				App->scene->gameObjects[lastComponent]->components[0]->owner = App->scene->gameObjects[lastComponent];
				App->scene->gameObjects[lastComponent]->components[1]->owner = App->scene->gameObjects[lastComponent];
				App->editor->AddLog("Car Created\n");
			}
			if (ImGui::MenuItem("Create Cube"))
			{
				showOptionsMenu = ComponentType::CUBE;
				position = { 0,0,0 };
				M.x = 1;
				M.y = 1;
				M.z = 1;
				
			}
			if (ImGui::MenuItem("Create Pyramid"))
			{
				showOptionsMenu = ComponentType::PYRAMID;
				position = { 0,0,0 };
				M.x = 1;
				M.y = 1;
				M.z = 1;
				
			}
			if (ImGui::MenuItem("Create Sphere"))
			{
				showOptionsMenu = ComponentType::SPHERE;
				position = { 0,0,0 };
				M.x = 1;
				M.y = 1;
				M.z = 1;
				
			}
			if (ImGui::MenuItem("Create Cylinder"))
			{
				showOptionsMenu = ComponentType::CYLINDER;
				position = { 0,0,0 };
				M.x = 1;
				M.y = 1;
				M.z = 1;
				
			}
			if (ImGui::MenuItem("Create Plane"))
			{
				showOptionsMenu = ComponentType::PLANE;
				position = { 0,0,0 };
				M.x = 1;
				M.y = 1;
				M.z = 1;
			}
			ImGui::EndMenu();
		}


		if (ImGui::BeginMenu("Help", &openConfigMenu))
		{
			if (ImGui::MenuItem("ImGui Demo"))
			{
				showDemoWindow = !showDemoWindow;
			}

			if (ImGui::MenuItem("Documentation"))
				ShellExecute(NULL, "open", "https://github.com/adrianam4/Chaos-Engine/wiki", NULL, NULL, SW_SHOWNORMAL);

			if (ImGui::MenuItem("Download Latest"))
				ShellExecute(NULL, "open", "https://github.com/adrianam4/Chaos-Engine/releases", NULL, NULL, SW_SHOWNORMAL);

			if (ImGui::MenuItem("Report a bug"))
				ShellExecute(NULL, "open", "https://github.com/adrianam4/Chaos-Engine/issues", NULL, NULL, SW_SHOWNORMAL);
		
			if (ImGui::MenuItem("About"))
			{
				showAboutWindow = !showAboutWindow;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Quit",&openConfigMenu))
		{
			return update_status::UPDATE_STOP;
		}
		ImGui::EndMainMenuBar();
	}

	//////////////////////////////////////////////////////////////////////////////////////////// HIERARCHY WINDOW ////////////////////////////////////////////////////////////////////////////////////////////

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		objectToChildren = nullptr;
		objectSelected = nullptr;
	}




	if (showHierarchy)
	{
		ImGui::CloseCurrentPopup();
		ImGui::Begin("Hierarchy", &showHierarchy);
		ImGui::LabelText("", "Game Objects in Scene: %d", App->scene->gameObjects.size());

		for (int i = 0; i < App->scene->gameObjects.size(); i++)
		{
			if (App->scene->gameObjects[i]->isChild == false) {
				if (ImGui::TreeNode(App->scene->gameObjects[i]->name.c_str()))
				{
					if (ImGui::IsItemHovered() && ImGui::GetIO().MouseClicked[0])
					{

						childrenManage(i);
					}



					for (int j = 0; j < App->scene->gameObjects[i]->childrens.size(); j++)
					{
						if (App->scene->gameObjects[i]->childrens.size() > 0)
						{
							if (ImGui::TreeNode(App->scene->gameObjects[i]->childrens[j]->name.c_str()))
							{
								if (ImGui::IsItemHovered() && ImGui::GetIO().MouseClicked[0])
								{
									objectSelected = App->scene->gameObjects[i]->childrens[j];
									//childrenManage(i);
								}
								ImGui::TreePop();
							}
						}
					}
					ImGui::TreePop();
				}
				else
				{

					if (ImGui::IsItemHovered() && ImGui::GetIO().MouseClicked[0])
					{
						childrenManage(i);


					}
				}
				ImGui::Separator();
			}
		}

		ImGui::End();
	}



	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_UP)
	{
		if (objectSelected && objectToChildren)
		{
			int a;
			for (a = 0; a < App->scene->gameObjects.size(); a++) {
				if (App->scene->gameObjects[a] == objectToChildren) {
					break;
				}
			}


			objectSelected->childrens.push_back(objectToChildren);
			objectToChildren->isChild = true;

			objectToChildren = nullptr;
			objectSelected = nullptr;
		}
		else {
			objectToChildren = nullptr;
			objectSelected = nullptr;
		}


	}

	if (showInspector && objectSelected != nullptr)
	{
		ImGui::Begin("Inspector", &showInspector);
		const char* name = objectSelected->name.c_str();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), name);
		ImGui::Separator();
		for (int i = 0; i < objectSelected->components.size(); i++)
		{
			if (ImGui::TreeNode(objectSelected->components[i]->name))
			{
				objectSelected->components[i]->OnEditor(i);

				ImGui::TreePop();
			}
		}
		ImGui::End();
	}

	//////////////////////////////////////////////////////////////////////////////////////////// CONFIGURATION WINDOW ////////////////////////////////////////////////////////////////////////////////////////////

	if (showConfigMenu)
	{
		ImGui::CloseCurrentPopup();
		ImGui::Begin("Configuration", &showConfigMenu);

		if (ImGui::CollapsingHeader("Application"))
		{
			static ImVector<float> fpsLog;
			static ImVector<float> msLog;
			static ImVector<float> memLog;
			char title[25];

			MEMORYSTATUSEX statex;

			statex.dwLength = sizeof(statex);

			GlobalMemoryStatusEx(&statex);

			fpsLog.push_back(ImGui::GetIO().Framerate);
			msLog.push_back(1000 / (ImGui::GetIO().Framerate));
			memLog.push_back(statex.ullTotalVirtual / DIV);

			ImGui::Text("%s", TITLE, 25);
			ImGui::Text("%s", ORGANIZATION, 25);
			if (ImGui::SliderInt("Max FPS", &maxFPS, 0, 144))
			{
				if (maxFPS > 0)
					App->maxMs = 1000 / maxFPS;
				else
					App->maxMs = 0;
			}

			sprintf_s(title, 25, "Framerate %.1f", fpsLog[fpsLog.size() - 1]);
			ImGui::PlotHistogram("##framerate", &fpsLog[0], fpsLog.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
			sprintf_s(title, 25, "Miliseconds %.1f", msLog[msLog.size() - 1]);
			ImGui::PlotHistogram("##miliseconds", &msLog[0], msLog.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
			/*sprintf_s(title, 25, "Memory Consumption %.1f", memLog[memLog.size() - 1]);
			ImGui::PlotHistogram("##memory", &memLog[0], memLog.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));*/


			ImGui::Text("Total Reported Mem: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d MB", statex.ullTotalPhys / DIV);
			/*ImGui::Text("Peak Reported Mem: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d MB");
			ImGui::Text("Accumulated Reported Mem: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d MB");*/
			ImGui::Text("Total Actual Mem: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d MB", statex.ullTotalVirtual / DIV);
			/*ImGui::Text("Peak Actual Mem: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d MB");
			ImGui::Text("Accumulated Actual Mem: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d MB");*/
			ImGui::Text("Total Alloc Unit Count: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d MB", statex.ullTotalVirtual / DIV - statex.ullAvailVirtual / DIV);
			/*ImGui::Text("Peak Alloc Unit Count: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d MB");
			ImGui::Text("Accumulated Alloc Unit Count: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d MB");*/
		}

		if (ImGui::CollapsingHeader("Window"))
		{
			if (ImGui::Checkbox("Active", &isActive))
			{

			}
			if (isActive)
			{

				ImGui::Text("Icon: *default*");

				if (ImGui::SliderFloat("Brigthness", &brightness, 0, 1.0f))
					SDL_SetWindowBrightness(App->window->window, brightness);
				if (ImGui::SliderInt("Width", &width, 0, 2560))
					SDL_SetWindowSize(App->window->window, width, height);
				if (ImGui::SliderInt("Heigh", &height, 0, 1440))
					SDL_SetWindowSize(App->window->window, width, height);
				
				int displayCount = 0, displayIndex = 0, modeIndex = 0;
				SDL_DisplayMode mode = { SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0 };

				if ((displayCount = SDL_GetNumVideoDisplays()) < 1) {
					SDL_Log("SDL_GetNumVideoDisplays returned: %i", displayCount);
				}

				if (SDL_GetDisplayMode(displayIndex, modeIndex, &mode) != 0) {
					SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
				}
				SDL_Log("SDL_GetDisplayMode(0, 0, &mode):\t\t%i bpp\t%i x %i",
					SDL_BITSPERPIXEL(mode.format), mode.w, mode.h);

				ImGui::Text("Refresh rate: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d", mode.refresh_rate);

				if (!resizable && !dekstop && !fullscreen && !borderless)
					resizable = true;

				if (ImGui::Checkbox("Fullscreen", &fullscreen))
				{
					App->window->SetFullscreen(fullscreen);
					resizable = false, borderless = false, dekstop = false;
				}
				ImGui::SameLine();
				if (ImGui::Checkbox("Resizable", &resizable))
				{
					if (fullscreen || borderless || dekstop)
					{
						App->window->SetResizable(resizable);
						fullscreen = false, borderless = false, dekstop = false;
					}
					if (!resizable)
						resizable = true;
				}

				if (ImGui::Checkbox("Borderless", &borderless))
				{
					App->window->SetBorderless(borderless);
					resizable = false, fullscreen = false, dekstop = false;
				}
				ImGui::SameLine();
				if (ImGui::Checkbox("Full Dekstop", &dekstop))
				{
					App->window->SetDekstop(dekstop);
					fullscreen = false, borderless = false, resizable = false;
				}
				if (ImGui::IsItemHovered())
				{
					ImGui::SetTooltip("Restart to apply");
				}
			}
		}

		if (ImGui::CollapsingHeader("File System"))
		{
			if (ImGui::Checkbox("Active", &isActive2))
			{
			}
			if (isActive2)
			{
				ImGui::Text("Base Path: ");
				ImGui::TextColored(ImVec4(255, 255, 0, 255), SDL_GetBasePath());

				ImGui::Text("Read Paths: ");
				ImGui::TextColored(ImVec4(255, 255, 0, 255), "%s", App->input->fileDir);

				ImGui::Text("Write Path: ");
				ImGui::TextColored(ImVec4(255, 255, 0, 255), "%s", "ConfigFile.json");
			}

		}

		if (ImGui::CollapsingHeader("Input"))
		{
			if (ImGui::Checkbox("Active", &isActive3))
			{

			}

			if (isActive3)
			{
				// Mouse Info
				ImGui::Text("Mouse Position: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d,%d", App->input->GetMouseX(), App->input->GetMouseY());
				ImGui::Text("Mouse Motion: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d,%d", App->input->GetMouseXMotion(), App->input->GetMouseYMotion());
				ImGui::Text("Mouse Wheel: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d", App->input->GetMouseZ());

				ImGui::Separator();

				//Keyboard Info
				static ImVector<int> keysId;
				static ImVector<const char*> keysState;

				for (int i = 0; i < 300; ++i)
				{
					if (App->input->keys[i] == 1)
					{
						if (App->input->keyboard[i] == KEY_IDLE)
						{
							keysId.push_back(i);
							keysState.push_back("DOWN");
						}
						else
						{
							keysId.push_back(i);
							keysState.push_back("REPEAT");
							break;
						}
					}
					else
					{
						if (App->input->keyboard[i] == KEY_REPEAT || App->input->keyboard[i] == KEY_DOWN)
						{
							keysId.push_back(i);
							keysState.push_back("UP");
						}
					}
				}

				for (int i = 0; i < keysId.Size; i++)
				{
					int j = keysState.Size - i - 1;
					ImGui::Text("Keybr: %d - %s", keysId[j], keysState[j]);
				}
			}
		}

		if (ImGui::CollapsingHeader("Hardware"))
		{
			if (ImGui::Checkbox("Active", &isActive4))
			{

			}

			if (isActive4)
			{
				SDL_version compiled;
				SDL_version linked;

				SDL_VERSION(&compiled);
				SDL_GetVersion(&linked);

				int cpuCores = SDL_GetCPUCount();
				int cpuCache = SDL_GetCPUCacheLineSize();
				float ram = SDL_GetSystemRAM() / 1000;

				ImGui::Text("SDL Version: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d.%d.%d", compiled.major, compiled.minor, compiled.patch);
				ImGui::Separator();
				ImGui::Text("CPUs: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d (Cache: %d kb)", cpuCores, cpuCache);
				ImGui::Text("System RAM: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "% .1f Gb", ram);
				ImGui::Text("Caps: ");
				ImGui::SameLine();
				if (SDL_HasRDTSC())
				{
					ImGui::TextColored(ImVec4(255, 255, 0, 255), "RDTSC, ");
				}
				ImGui::SameLine();
				if (SDL_HasMMX())
				{
					ImGui::TextColored(ImVec4(255, 255, 0, 255), "MMX, ");
				}
				ImGui::SameLine();
				if (SDL_HasSSE())
				{
					ImGui::TextColored(ImVec4(255, 255, 0, 255), "SSE, ");
				}
				ImGui::SameLine();
				if (SDL_HasSSE2())
				{
					ImGui::TextColored(ImVec4(255, 255, 0, 255), "SSE2, ");
				}
				if (SDL_HasSSE3())
				{
					ImGui::TextColored(ImVec4(255, 255, 0, 255), "SSE3, ");
				}
				ImGui::SameLine();
				if (SDL_HasSSE41())
				{
					ImGui::TextColored(ImVec4(255, 255, 0, 255), "SSE41, ");
				}
				ImGui::SameLine();
				if (SDL_HasSSE42())
				{
					ImGui::TextColored(ImVec4(255, 255, 0, 255), "SSE42, ");
				}
				ImGui::SameLine();
				if (SDL_HasAVX())
				{
					ImGui::TextColored(ImVec4(255, 255, 0, 255), "AVX, ");
				}
				ImGui::SameLine();
				if (SDL_HasAVX2())
				{
					ImGui::TextColored(ImVec4(255, 255, 0, 255), "AVX2, ");
				}
				ImGui::Separator();

				const GLubyte* vendor = glGetString(GL_VENDOR);
				const GLubyte* renderer = glGetString(GL_RENDERER);
				GLint gpuInfo = 0;

				MEMORYSTATUSEX statex;

				statex.dwLength = sizeof(statex);

				GlobalMemoryStatusEx(&statex);

				GLint total_mem_kb = 0;
				glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX,
					&total_mem_kb);

				GLint cur_avail_mem_kb = 0;
				glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX,
					&cur_avail_mem_kb);

				ImGui::Text("GPUs: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%s", renderer);
				ImGui::Text("Brand: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%s", vendor);
				ImGui::Text("VRAM Budget: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d Mb", total_mem_kb / 1000);
				ImGui::Text("VRAM Usage: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d Mb", ((total_mem_kb - cur_avail_mem_kb)) / 1000);
				ImGui::Text("VRAM Available: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d Mb", cur_avail_mem_kb / 1000);
				//ImGui::Text("VRAM Reserved: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d Kb");
			}
		}

		ImGui::End();
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////// CONSOLE WINDOW ////////////////////////////////////////////////////////////////////////////////////////////
	
	if (showConsoleMenu)
	{
		ImGui::CloseCurrentPopup();
		ImGui::Begin("Console", &showConsoleMenu);

		ImGui::TextUnformatted(consoleBuffer.begin());

		if (scrollToBottom)
			ImGui::SetScrollHereY(1.0f);
		
		scrollToBottom = false;
		ImGui::End();
	}

	if (showAboutWindow)
	{
		ImGui::CloseCurrentPopup();
		ImGui::Begin("About", &showAboutWindow);

		ImGui::Text("Chaos Engine v0.1");
		ImGui::Separator();
		ImGui::Text("A trully amazing 3D Game Engine.");
		ImGui::Text("Created by David Lira, Adrian Aroca and Unai Diaz for the Game Engines subject.");
		ImGui::Text("David Lira Github: https://github.com/davidlira19");
		ImGui::Text("Adrian Aroca Github: https://github.com/adrianam4");
		ImGui::Text("Unai Diaz Github: https://github.com/unaidiaz");
		ImGui::Separator();
		ImGui::Text("3rd Party Libraries used:");
		ImGui::Text("");
		SDL_version compiled; SDL_version linked; SDL_VERSION(&compiled); SDL_GetVersion(&linked);
		ImGui::Text("- SDL v%d.%d.%d",compiled.major, compiled.minor, compiled.patch);
		ImGui::Text("- Glew v7.0");
		ImGui::Text("- ImGui v1.84.2");
		ImGui::Text("- MathGeoLib v1.5");
		ImGui::Text("- DevIL v1.8.0");
		ImGui::Text("- OpenGL v%s",glGetString(GL_VERSION));
		ImGui::Text("");
		ImGui::Separator();
		ImGui::Text("License:");
		ImGui::Text("");
		ImGui::Text("MIT License");
		ImGui::Text("");
		ImGui::Text("Copyright (c) 2021 adrianam4, davidlira19 and unaidiaz");
		ImGui::Text("");
		ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy");
		ImGui::Text("of this software and associated documentation files (the ''Software''), to deal");
		ImGui::Text("in the Software without restriction, including without limitation the rights");
		ImGui::Text("to use, copy, modify, merge, publish, distribute, sublicense, and/or sell");
		ImGui::Text("copies of the Software, and to permit persons to whom the Software is");
		ImGui::Text("furnished to do so, subject to the following conditions:");
		ImGui::Text("");
		ImGui::Text("The above copyright notice and this permission notice shall be included in all");
		ImGui::Text("copies or substantial portions of the Software.");
		ImGui::Text("");
		ImGui::Text("THE SOFTWARE IS PROVIDED ''AS IS'', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR");
		ImGui::Text("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,");
		ImGui::Text("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE");
		ImGui::Text("AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER");
		ImGui::Text("LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,");
		ImGui::Text("OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE");
		ImGui::Text("SOFTWARE.");

		ImGui::End();
	}

	if (showDemoWindow)
		ImGui::ShowDemoWindow(&showDemoWindow);

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate(float dt)
{
	grid->DrawGrid();

	for (int i = 0; i < App->scene->gameObjects.size(); i++)
	{
		if (showAABB)
		{
			for (int a = 0; a < App->scene->gameObjects[i]->boundingBoxes.size(); a++)
			{
				App->scene->gameObjects[i]->boundingBoxes[a]->DrawCube();
			}
		}

		for (int j = 0; j < App->scene->gameObjects[i]->components.size(); j++)
		{
			//Cubes
			if (App->scene->gameObjects[i]->components[j]->type == ComponentType::CUBE && App->scene->gameObjects[i]->components[j]->active)
			{
				int auxId = App->scene->gameObjects[i]->id;
				for (int k = 0; k < cubes.size(); k++)
				{
					if (cubes[k]->id == auxId)
					{
						cubes[k]->DrawCube();
					}
				}
			}
			//Pyranids
			if (App->scene->gameObjects[i]->components[j]->type == ComponentType::PYRAMID && App->scene->gameObjects[i]->components[j]->active)
			{
				int auxId = App->scene->gameObjects[i]->id;
				for (int k = 0; k < pyramids.size(); k++)
				{
					if (pyramids[k]->id == auxId)
					{
						pyramids[k]->DrawPyramid();
					}
				}
			}
			//Cylinders
			if (App->scene->gameObjects[i]->components[j]->type == ComponentType::CYLINDER && App->scene->gameObjects[i]->components[j]->active)
			{
				int auxId = App->scene->gameObjects[i]->id;
				for (int k = 0; k < cylinders.size(); k++)
				{
					if (cylinders[k]->id == auxId)
					{
						cylinders[k]->DrawCylinder();
					}
				}
			}
			//Spheres
			if (App->scene->gameObjects[i]->components[j]->type == ComponentType::SPHERE && App->scene->gameObjects[i]->components[j]->active)
			{
				int auxId = App->scene->gameObjects[i]->id;
				for (int k = 0; k < spheres.size(); k++)
				{
					if (spheres[k]->id == auxId)
					{
						spheres[k]->DrawSphere();
					}
				}
			}
			//Planes
			if (App->scene->gameObjects[i]->components[j]->type == ComponentType::PLANE && App->scene->gameObjects[i]->components[j]->active)
			{
				int auxId = App->scene->gameObjects[i]->id;
				for (int k = 0; k < planes.size(); k++)
				{
					if (planes[k]->id == auxId)
					{
						planes[k]->DrawPlane();
					}
				}
			}
			
		}
	}

	return UPDATE_CONTINUE;
}

void  ModuleEditor::childrenManage(int i) {

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_REPEAT)
	{

		if (objectSelected == nullptr && objectToChildren == nullptr)
		{
			objectToChildren = App->scene->gameObjects[i];
		}
		else if (objectSelected == nullptr && objectToChildren) {
			objectSelected = App->scene->gameObjects[i];
		}
		else if (objectSelected && objectToChildren) {
			objectToChildren = App->scene->gameObjects[i];
			objectSelected = nullptr;
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_P) == KEY_IDLE) {
		objectSelected = App->scene->gameObjects[i];
	}
}