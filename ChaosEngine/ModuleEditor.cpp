#include "Application.h"
#include "Globals.h"
#include "ModuleEditor.h"
#include "ModuleScene.h"
#include "Mesh.h"
#include "Importer.h"
#include "shellapi.h"
#include <GL/GL.h>
#include "ImGuizmo.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
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

	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	sceneWindow |= ImGuiWindowFlags_NoScrollbar;

	// Setup ImGui style by default
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplOpenGL3_Init();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);

	return ret;
}

//update_status ModuleEditor::PreUpdate(float dt) {
//
//	// Start the Dear ImGui frame
//	ImGui_ImplOpenGL3_NewFrame();
//	ImGui_ImplSDL2_NewFrame(App->window->window);
//	ImGui::NewFrame();
//
//	return UPDATE_CONTINUE;
//
//}

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

bool ModuleEditor::DockingRootItem(char* id, ImGuiWindowFlags winFlags)
{
	//Setting windows as viewport size
	ImGuiViewport* viewport = ImGui::GetWindowViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);

	//Setting window style
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, .0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, .0f);

	//Viewport window flags just to have a non interactable white space where we can dock the rest of windows
	winFlags |= ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar;
	winFlags |= ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground;

	bool temp = true;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	temp = ImGui::Begin(id, &temp, winFlags);
	ImGui::PopStyleVar(3);

	BeginDock(id, ImGuiDockNodeFlags_PassthruCentralNode);

	return temp;
}

void ModuleEditor::BeginDock(char* dockSpaceId, ImGuiDockNodeFlags dockFlags, ImVec2 size)
{
	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
		ImGuiID dock = ImGui::GetID(dockSpaceId);
		ImGui::DockSpace(dock, size, dockFlags);
	}
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

	json_serialize_to_file_pretty(user_data, "Settings/ConfigFile.json");

	AddLog("Saved Config Data\n");
}

void ModuleEditor::LoadConfig()
{
	//Reading JSON File
	JSON_Value* userData = json_parse_file("Settings/ConfigFile.json");

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

void ModuleEditor::SaveScene()
{
	JSON_Value* root = json_value_init_object();
	JSON_Value* arr = json_value_init_array();
	JSON_Object* rootObj = json_value_get_object(root);
	JSON_Array* myArray = json_value_get_array(arr);
	GameObject* go;

	json_object_dotset_value(rootObj, "GameObjects", arr);
	for (int i = 0; i < App->scene->gameObjects.size(); i++)
	{
		go = App->scene->gameObjects[i];
		JSON_Value* gameObjectValue = json_value_init_object();
		JSON_Object* gameObjObject = json_value_get_object(gameObjectValue);
		json_object_set_number(gameObjObject, "UID", go->UID);
		json_object_set_string(gameObjObject, "Name", go->name.c_str());

		for (int j = 0; j < go->components.size(); j++)
		{
			if (go->components[j]->type == ComponentType::MESH)
			{
				json_object_dotset_number(gameObjObject, "Components.Mesh.Type", (double)go->components[j]->type);
				json_object_dotset_number(gameObjObject, "Components.Mesh.UID", go->components[j]->UID);
				json_object_dotset_string(gameObjObject, "Components.Mesh.Path", go->components[j]->modelPath);
				json_object_dotset_boolean(gameObjObject, "Components.Mesh.Active", go->components[j]->active);
				json_object_dotset_boolean(gameObjObject, "Components.Mesh.Normals", go->components[j]->drawNormals);
				json_object_dotset_boolean(gameObjObject, "Components.Mesh.Wireframe", go->components[j]->drawWireframe);
			}
			if (go->components[j]->type == ComponentType::CUBE || go->components[j]->type == ComponentType::CYLINDER || go->components[j]->type == ComponentType::PLANE
				|| go->components[j]->type == ComponentType::PYRAMID || go->components[j]->type == ComponentType::SPHERE)
			{
				json_object_dotset_number(gameObjObject, "Components.Mesh.Type", (double)go->components[j]->type);
				json_object_dotset_number(gameObjObject, "Components.Mesh.UID", go->components[j]->UID);
				json_object_dotset_boolean(gameObjObject, "Components.Mesh.Active", go->components[j]->active);
				json_object_dotset_boolean(gameObjObject, "Components.Mesh.Wireframe", go->components[j]->drawWireframe);
			}
			if (go->components[j]->type == ComponentType::TRANSFORM)
			{
				json_object_dotset_number(gameObjObject, "Components.Transform.Type", (double)go->components[j]->type);
				json_object_dotset_number(gameObjObject, "Components.Transform.UID", go->components[j]->UID);
				json_object_dotset_number(gameObjObject, "Components.Transform.Position.x", go->components[j]->position.x);
				json_object_dotset_number(gameObjObject, "Components.Transform.Position.y", go->components[j]->position.y);
				json_object_dotset_number(gameObjObject, "Components.Transform.Position.z", go->components[j]->position.z);

				json_object_dotset_number(gameObjObject, "Components.Transform.GeneralScale", go->components[j]->generalScale);
				json_object_dotset_number(gameObjObject, "Components.Transform.Scale.x", go->components[j]->scale.x);
				json_object_dotset_number(gameObjObject, "Components.Transform.Scale.y", go->components[j]->scale.y);
				json_object_dotset_number(gameObjObject, "Components.Transform.Scale.z", go->components[j]->scale.z);

				json_object_dotset_number(gameObjObject, "Components.Transform.Rotation.x", go->components[j]->rotationEuler.x);
				json_object_dotset_number(gameObjObject, "Components.Transform.Rotation.y", go->components[j]->rotationEuler.y);
				json_object_dotset_number(gameObjObject, "Components.Transform.Rotation.z", go->components[j]->rotationEuler.z);
			}
			if (go->components[j]->type == ComponentType::MATERIAL)
			{
				json_object_dotset_number(gameObjObject, "Components.Material.Type", (double)go->components[j]->type);
				json_object_dotset_number(gameObjObject, "Components.Material.UID", go->components[j]->UID);
				json_object_dotset_string(gameObjObject, "Components.Material.TexturePath", go->components[j]->texturePath);
				json_object_dotset_number(gameObjObject, "Components.Material.Width", go->components[j]->width);
				json_object_dotset_number(gameObjObject, "Components.Material.Height", go->components[j]->height);
			}
			if (go->components[j]->type == ComponentType::CAMERA)
			{
				json_object_dotset_number(gameObjObject, "Components.Camera.Type", (double)go->components[j]->type);
				json_object_dotset_number(gameObjObject, "Components.Camera.UID", go->components[j]->UID);
				json_object_dotset_number(gameObjObject, "Components.Camera.FOV", go->components[j]->horizontalFov);
				json_object_dotset_number(gameObjObject, "Components.Camera.NearPlane", go->components[j]->nearPlaneDistance);
				json_object_dotset_number(gameObjObject, "Components.Camera.FarPlane", go->components[j]->farPlaneDistance);
			}
		}
		json_array_append_value(myArray, gameObjectValue);
	}

	json_serialize_to_file_pretty(root, "Assets/SceneFile.json");

	AddLog("Saved Scene Data\n");
}

void ModuleEditor::LoadScene()
{
	JSON_Value* root = json_parse_file("Assets/SceneFile.json");
	JSON_Array* gameObjectsInfo = json_object_dotget_array(json_value_get_object(root), "GameObjects");

	for (size_t i = 0; i < json_array_get_count(gameObjectsInfo); i++)
	{
		JSON_Value* auxValue = json_array_get_value(gameObjectsInfo, i);

		u32 UID = json_object_get_number(json_value_get_object(auxValue), "UID");
		const char* name = json_object_get_string(json_value_get_object(auxValue), "Name");

		bool createGameObject = true;
		for (size_t j = 0; j < App->scene->gameObjects.size(); j++)
		{
			if (App->scene->gameObjects[j]->UID == UID)
			{
				createGameObject = false;
				break;
			}
		}
		if (createGameObject)
		{
			std::string futureNumber = std::string(name);
			size_t start = futureNumber.find_last_of("_");
			futureNumber = futureNumber.substr(start + 1, futureNumber.length() - start);
			App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, atoi(futureNumber.c_str()), name)); // Create GO with name
			objectSelected = App->scene->gameObjects[App->scene->gameObjects.size() - 1];
			GameObject* lastGO = App->scene->gameObjects[App->scene->gameObjects.size() - 1];
			lastGO->UID = UID; //UID
			/*  MESH   */
			int auxType = json_object_dotget_number(json_value_get_object(auxValue), "Components.Mesh.Type");
			u32 compUID = json_object_dotget_number(json_value_get_object(auxValue), "Components.Mesh.UID");
			bool wire = json_object_dotget_boolean(json_value_get_object(auxValue), "Components.Mesh.Wireframe");
			bool act = json_object_dotget_boolean(json_value_get_object(auxValue), "Components.Mesh.Active");
			ComponentType type;
			if (auxType == 2)
			{
				bool nor = json_object_dotget_boolean(json_value_get_object(auxValue), "Components.Mesh.Normals");
				const char* modelPath = json_object_dotget_string(json_value_get_object(auxValue), "Components.Mesh.Path");

				FBXmporter importer;

				std::string firstNum = std::string(modelPath);
				unsigned firstNumPos = firstNum.find_last_of("/");
				firstNum = firstNum.substr(firstNumPos+1+8, 1);

				std::string Uid = std::string(modelPath);
				unsigned UidNumPos = Uid.find_last_of("/");
				Uid = Uid.substr(UidNumPos + 1, 8);

				std::string secondNum = std::string(modelPath);
				unsigned secondNumPos = secondNum.find_last_of("/");
				secondNum = secondNum.substr(secondNumPos + 2+8, 1);

				lastGO->components.push_back(lastGO->CreateMeshComponent(importer.loadFromOurFile("Library/Models/", Uid.c_str(), firstNum.c_str(), secondNum.c_str(), ".msh"), modelPath));
				//lastGO->components.push_back(lastGO->CreateComponent(ComponentType::MESH, modelPath, false));
			}
			if (auxType == 3)
				lastGO->components.push_back(lastGO->CreateComponent(ComponentType::CUBE, &float3(0, 0, 0), &float3(1, 1, 1), &float3(0, 0, 0)));
			if (auxType == 6)
				lastGO->components.push_back(lastGO->CreateComponent(ComponentType::CYLINDER, &float3(0, 0, 0), &float3(1, 1, 1), &float3(0, 0, 0)));
			if (auxType == 7)
				lastGO->components.push_back(lastGO->CreateComponent(ComponentType::PLANE, &float3(0, 0, 0), &float3(1, 1, 1), &float3(0, 0, 0)));
			if (auxType == 4)
				lastGO->components.push_back(lastGO->CreateComponent(ComponentType::PYRAMID, &float3(0, 0, 0), &float3(1, 1, 1), &float3(0, 0, 0)));
			if (auxType == 5)
				lastGO->components.push_back(lastGO->CreateComponent(ComponentType::SPHERE, &float3(0, 0, 0), &float3(1, 1, 1), &float3(0, 0, 0)));

			/*   TRANSFORMATIONS   */
			//Position
			double position_x = json_object_dotget_number(json_value_get_object(auxValue), "Components.Transform.Position.x");
			double position_y = json_object_dotget_number(json_value_get_object(auxValue), "Components.Transform.Position.y");
			double position_z = json_object_dotget_number(json_value_get_object(auxValue), "Components.Transform.Position.z");
			float3 position = float3(position_x, position_y, position_z);
			//Scale
			double scale_x = json_object_dotget_number(json_value_get_object(auxValue), "Components.Transform.Scale.x");
			double scale_y = json_object_dotget_number(json_value_get_object(auxValue), "Components.Transform.Scale.y");
			double scale_z = json_object_dotget_number(json_value_get_object(auxValue), "Components.Transform.Scale.z");
			float3 scale = float3(scale_x, scale_y, scale_z);
			//Rotation
			double rotation_x = json_object_dotget_number(json_value_get_object(auxValue), "Components.Transform.Rotation.x");
			double rotation_y = json_object_dotget_number(json_value_get_object(auxValue), "Components.Transform.Rotation.y");
			double rotation_z = json_object_dotget_number(json_value_get_object(auxValue), "Components.Transform.Rotation.z");
			float3 rotation = float3(rotation_x, rotation_y, rotation_z);
			
			lastGO->components.push_back(lastGO->CreateComponent(ComponentType::TRANSFORM, &position, &scale, &rotation));

			/*   MATERIALS   */
			auxType = json_object_dotget_number(json_value_get_object(auxValue), "Components.Material.Type");
			if (auxType==8) 
			{
				u32 materialUID = json_object_dotget_number(json_value_get_object(auxValue), "Components.Material.UID");
				const char* textPath = json_object_dotget_string(json_value_get_object(auxValue), "Components.Material.TexturePath");
				double width = json_object_dotget_number(json_value_get_object(auxValue), "Components.Material.Width");
				double height = json_object_dotget_number(json_value_get_object(auxValue), "Components.Material.Height");

				lastGO->components.push_back(lastGO->CreateComponent(ComponentType::MATERIAL, textPath, false));
			}

			/*   CAMERAS   */
			u32 cameraUID = json_object_dotget_number(json_value_get_object(auxValue), "Components.Camera.UID");
			int Type = json_object_dotget_number(json_value_get_object(auxValue), "Components.Camera.Type");
			double horizontalFov = json_object_dotget_number(json_value_get_object(auxValue), "Components.Camera.FOV");
			double nearPlane = json_object_dotget_number(json_value_get_object(auxValue), "Components.Camera.NearPlane");
			double farPlane = json_object_dotget_number(json_value_get_object(auxValue), "Components.Camera.FarPlane");
			lastGO->components.push_back(lastGO->CreateComponent2(ComponentType::CAMERA, position, horizontalFov, nearPlane, farPlane));
		}
	}
	
	UpdateAll();

	AddLog("Loaded Scene Data\n");
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
void ModuleEditor::UpdateAll()
{
	for (size_t i = 0; i < App->scene->gameObjects.size(); i++)
	{
		for (size_t j = 0; j < App->scene->gameObjects[i]->components.size(); j++)
		{
			if (App->scene->gameObjects[i]->components[j]->type == ComponentType::TRANSFORM)
			{
				App->editor->objectSelected = App->scene->gameObjects[i];
				App->scene->gameObjects[i]->components[j]->Update();
			}	
		}
	}
	App->editor->objectSelected = nullptr;
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
			objectSelected = App->scene->gameObjects[lastComponent];
			App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::CUBE, &position,&M,&float3(0,0,0)));
			App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::TRANSFORM, &position, &M, &float3(0, 0, 0)));
			App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent2(ComponentType::CAMERA, position, 75, 5, 100));
			App->scene->gameObjects[lastComponent]->components[0]->owner = App->scene->gameObjects[lastComponent];
			App->scene->gameObjects[lastComponent]->components[1]->owner = App->scene->gameObjects[lastComponent];
			App->scene->gameObjects[lastComponent]->components[2]->owner = App->scene->gameObjects[lastComponent];
			
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
			objectSelected = App->scene->gameObjects[lastComponent];
			App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::PYRAMID, &position, &M, &float3(0, 0, 0)));
			App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::TRANSFORM, &position, &M, &float3(0, 0, 0)));
			App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent2(ComponentType::CAMERA, position, 75, 5, 100));
			App->scene->gameObjects[lastComponent]->components[0]->owner = App->scene->gameObjects[lastComponent];
			App->scene->gameObjects[lastComponent]->components[1]->owner = App->scene->gameObjects[lastComponent];
			App->scene->gameObjects[lastComponent]->components[2]->owner = App->scene->gameObjects[lastComponent];
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
			objectSelected = App->scene->gameObjects[lastComponent];
			App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::CYLINDER, &position, &M, &float3(0, 0, 0)));
			App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::TRANSFORM, &position, &M, &float3(0, 0, 0)));
			App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent2(ComponentType::CAMERA, position, 75, 5, 100));
			App->scene->gameObjects[lastComponent]->components[0]->owner = App->scene->gameObjects[lastComponent];
			App->scene->gameObjects[lastComponent]->components[1]->owner = App->scene->gameObjects[lastComponent];
			App->scene->gameObjects[lastComponent]->components[2]->owner = App->scene->gameObjects[lastComponent];
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
			objectSelected = App->scene->gameObjects[lastComponent];
			App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::SPHERE, &position, &M, &float3(0, 0, 0)));
			App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::TRANSFORM, &position, &M, &float3(0, 0, 0)));
			App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent2(ComponentType::CAMERA, position, 75, 5, 100));
			App->scene->gameObjects[lastComponent]->components[0]->owner = App->scene->gameObjects[lastComponent];
			App->scene->gameObjects[lastComponent]->components[1]->owner = App->scene->gameObjects[lastComponent];
			App->scene->gameObjects[lastComponent]->components[2]->owner = App->scene->gameObjects[lastComponent];
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
			objectSelected = App->scene->gameObjects[lastComponent];
			App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::PLANE, &position, &M, &float3(0, 0, 0)));
			App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::TRANSFORM, &position, &M, &float3(0, 0, 0)));
			App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent2(ComponentType::CAMERA, position, 75, 5, 100));
			App->scene->gameObjects[lastComponent]->components[0]->owner = App->scene->gameObjects[lastComponent];
			App->scene->gameObjects[lastComponent]->components[1]->owner = App->scene->gameObjects[lastComponent];
			App->scene->gameObjects[lastComponent]->components[2]->owner = App->scene->gameObjects[lastComponent];
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
	grid->DrawGrid();
	//Creating MenuBar item as a root for docking windows

	//////////////////////////////////////////////////////////////////////////// GUIZMOS ///////////////////////////////////////////////////////////////////////
	/*if (objectSelected)
	{
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();
		float windowWidth = (float)ImGui::GetWindowWidth();
		float windowHeight = (float)ImGui::GetWindowHeight();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

		auto cameraEntity = App->camera->GetViewMatrix();
		const auto& camera = cameraEntity->GetComponentType();


		auto& tc = objectSelected->components;

		ImGuizmo::Manipulate(App->camera->GetViewMatrix(), App->camera->GetViewMatrix(),
			ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::LOCAL, App->camera->GetViewMatrix());

	}*/

	//int gizmoCount = 1;
	//static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
	//static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
	//static bool useSnap = false;
	//static float snap[3] = { 1.f, 1.f, 1.f };
	//static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
	//static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
	//static bool boundSizing = false;
	//static bool boundSizingSnap = false;

	//ImGuizmo::SetDrawlist();
	//ImGuizmo::BeginFrame();

	//if (objectSelected != nullptr && App->input->GetKey(SDL_SCANCODE_U) == KEY_DOWN)
	//	mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	//if (objectSelected != nullptr && App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
	//	mCurrentGizmoOperation = ImGuizmo::ROTATE;
	//if (objectSelected != nullptr && App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
	//	mCurrentGizmoOperation = ImGuizmo::SCALE;


	if (DockingRootItem("Viewport", ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
	}

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
	static bool openSceneMenu = false;
	static bool showConfigMenu = true;
	static bool showWarningMenu = false;
	static bool showHierarchy = true;
	static bool showInspector = true;
	static bool showConsoleMenu = true;
	static bool showSceneWindow = true;
	static bool showOptions = true;
	static bool isActive = true;
	static bool isActive2 = true;
	static bool isActive3 = true;
	static bool isActive4 = true;
	ImVec4 clearColor = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

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
			if (ImGui::MenuItem("Save Config"))
			{
				SaveConfig();
			}
			if (ImGui::MenuItem("Load Config"))
			{
				LoadConfig();
				ComproveScreen();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Scene", &openSceneMenu))
		{
			if (ImGui::MenuItem("Save Scene"))
			{
				SaveScene();
			}
			if (ImGui::MenuItem("Load Scene"))
			{
				showWarningMenu = !showWarningMenu;
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
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::TRANSFORM, &float3(0, 0, 0), &float3(1, 1, 1), &float3(0, 0, 0)));
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent2(ComponentType::CAMERA, float3(0, 0, 0), 75, 5, 100));
				App->scene->gameObjects[lastComponent]->components[0]->owner = App->scene->gameObjects[lastComponent];
				App->scene->gameObjects[lastComponent]->components[1]->owner = App->scene->gameObjects[lastComponent];
				App->scene->gameObjects[lastComponent]->components[2]->owner = App->scene->gameObjects[lastComponent];
				objectSelected = App->scene->gameObjects[lastComponent];
				App->editor->AddLog("Empty Object Created\n");
			}
			if (ImGui::MenuItem("Create House"))
			{
				static int houses = 2;
				App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, houses, "House "));
				houses++;
				int lastComponent = App->scene->gameObjects.size() - 1;
				objectSelected = App->scene->gameObjects[lastComponent];
				FBXmporter importer;
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateMeshComponent(importer.loadFromOurFile("Library/Models/","29403208", "0", "2", ".msh"), "Library/Models/2940320802.msh"));
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::TRANSFORM,&float3(0,0,0),&float3(1,1,1),&float3(0,0,0)));
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::MATERIAL, "Assets/Textures/BakerHouse.png", false));
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent2(ComponentType::CAMERA, float3(0, 0, 0), 75, 5, 100));
				App->scene->gameObjects[lastComponent]->components[0]->owner = App->scene->gameObjects[lastComponent];
				App->scene->gameObjects[lastComponent]->components[1]->owner = App->scene->gameObjects[lastComponent];
				App->scene->gameObjects[lastComponent]->components[2]->owner = App->scene->gameObjects[lastComponent];
				
				App->editor->AddLog("House Created\n");
			}
			if (ImGui::MenuItem("Create Penguin"))
			{
				static int penguins = 1;
				App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, penguins, "Penguin "));
				penguins++;
				int lastComponent = App->scene->gameObjects.size() - 1;
				objectSelected = App->scene->gameObjects[lastComponent];
				FBXmporter importer;
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateMeshComponent(importer.loadFromOurFile("Library/Models/","21274500", "0", "1", ".msh"), "Library/Models/2127450001.msh"));
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::TRANSFORM, &float3(0, 0, 0), &float3(1, 1, 1), &float3(0, 0, 0)));
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::MATERIAL, "Library/Textures/Penguin.dds", false));
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent2(ComponentType::CAMERA, float3(0, 0, 0), 75, 5, 100));
				App->scene->gameObjects[lastComponent]->components[0]->owner = App->scene->gameObjects[lastComponent];
				App->scene->gameObjects[lastComponent]->components[1]->owner = App->scene->gameObjects[lastComponent];
				App->scene->gameObjects[lastComponent]->components[2]->owner = App->scene->gameObjects[lastComponent];
				
				App->editor->AddLog("Penguin Created\n");
			}
			if (ImGui::MenuItem("Create Car"))
			{
				static int cars = 1;
				App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, cars, "Car "));
				
				cars++;
				int lastComponent = App->scene->gameObjects.size() - 1;
				objectSelected = App->scene->gameObjects[lastComponent];
				FBXmporter importer;
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateMeshComponent(importer.loadFromOurFile("Library/Models/","21854936", "0", "5", ".msh"), "Library/Models/2185493605.msh"));
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::TRANSFORM, &float3(0, 0, 0), &float3(1, 1, 1), &float3(0, 0, 0)));
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::MATERIAL, "Library/Textures/Car.dds", false));
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent2(ComponentType::CAMERA, float3(0, 0, 0), 75, 5, 100));
				App->scene->gameObjects[lastComponent]->components[0]->owner = App->scene->gameObjects[lastComponent];
				App->scene->gameObjects[lastComponent]->components[1]->owner = App->scene->gameObjects[lastComponent];
				App->scene->gameObjects[lastComponent]->components[2]->owner = App->scene->gameObjects[lastComponent];
				
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

	//////////////////////////////////////////////////////////////////////////////////////////// VIEWPORT WINDOW ////////////////////////////////////////////////////////////////////////////////////////////

	if (showSceneWindow)
	{
		ImGui::CloseCurrentPopup();
		ImGui::Begin("Scene", &showSceneWindow, ImGuiWindowFlags_NoScrollbar);

		ImVec2 viewportSize = ImGui::GetCurrentWindow()->Size;
		if (viewportSize.x != lastViewportSize.x || viewportSize.y != lastViewportSize.y)
		{
			App->camera->aspectRatio = viewportSize.x / viewportSize.y;
			App->camera->RecalculateProjection();
		}
		lastViewportSize = viewportSize;
		ImGui::Image((ImTextureID)App->viewportBuffer->texture, viewportSize, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
	}

	//////////////////////////////////////////////////////////////////////////////////////////// WARNING WINDOW ////////////////////////////////////////////////////////////////////////////////////////////

	if (showWarningMenu)
	{
		ImGui::CloseCurrentPopup();
		ImGui::Begin("Warning", &showWarningMenu);

		ImGui::Text("Are you sure you want to load the saved scene?");
		ImGui::Text("(All actions after the save will be deleted)");

		if (ImGui::MenuItem("Yes"))
		{
			for (int i = 0; i <= App->scene->gameObjects.size(); i++)
			{
				if (App->scene->gameObjects.size() > 0) 
					App->scene->gameObjects.erase(App->scene->gameObjects.begin());
			}
			LoadScene();
			showWarningMenu = !showWarningMenu;
		}
		if (ImGui::MenuItem("No"))
		{
			showWarningMenu = !showWarningMenu;
		}

		ImGui::End();
	}

	if (showDemoWindow)
		ImGui::ShowDemoWindow(&showDemoWindow);


	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate(float dt)
{
	grid->DrawGrid();

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Rendering
	ImGui::Render();
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);


	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}

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