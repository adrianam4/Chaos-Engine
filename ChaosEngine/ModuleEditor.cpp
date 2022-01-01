#include "Application.h"
#include "Globals.h"
#include "ModuleEditor.h"
#include "ModuleScene.h"
#include "Mesh.h"
#include "Importer.h"
#include "GameTime.h"
#include "shellapi.h"
#include <GL/GL.h>
#include "ImGuizmo.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "Parson/parson.h"
#include "CameraComponent.h"
#include "Transform2DComponent.h"
#include "MaterialComponent.h"
#include "ResourceMaterial.h"
#include "ResourceMesh.h"
#include "Component.h"
#include "FileDialog.h"
#include"MeshComponent.h"
#include<stack>

#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

#define DIV 1048576

#define TOTEX (void*)(intptr_t)

ModuleEditor::ModuleEditor(Application* app, bool startEnabled) : Module(app, startEnabled)
{
	lastId = -1;
	objectSelected = nullptr;
	consoleBuffer.clear();
	showSaveOnExitMenu = false;
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

	float fontSize = 18.0f;// *2.0f;
	io.Fonts->AddFontFromFileTTF("Assets/Fonts/OpenSans-Bold.ttf", fontSize);
	io.FontDefault = io.Fonts->AddFontFromFileTTF("Assets/Fonts/OpenSans-Regular.ttf", fontSize);

	// Setup ImGui style by default
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplOpenGL3_Init();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);

	ImGuiStyle* style = &ImGui::GetStyle();

	style->WindowTitleAlign = ImVec2(0.5, 0.5);

	style->Colors[ImGuiCol_TitleBg] = ImColor(200, 200, 200, 255);
	style->Colors[ImGuiCol_TitleBgActive] = ImColor(200, 200, 200, 255);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImColor(0, 0, 0, 130);

	style->Colors[ImGuiCol_Button] = ImColor(31, 30, 31, 0);
	style->Colors[ImGuiCol_ButtonActive] = ImColor(31, 30, 31, 255);
	style->Colors[ImGuiCol_ButtonHovered] = ImColor(41, 40, 41, 255);

	style->Colors[ImGuiCol_Separator] = ImColor(70, 70, 70, 255);
	style->Colors[ImGuiCol_SeparatorActive] = ImColor(70, 70, 70, 255);
	style->Colors[ImGuiCol_SeparatorHovered] = ImColor(76, 76, 76, 255);

	style->Colors[ImGuiCol_Header] = ImColor(70, 70, 70, 255);
	style->Colors[ImGuiCol_HeaderActive] = ImColor(70, 70, 70, 255);
	style->Colors[ImGuiCol_HeaderHovered] = ImColor(76, 76, 76, 255);

	style->Colors[ImGuiCol_Tab] = ImColor(70, 70, 70, 255);
	style->Colors[ImGuiCol_TabActive] = ImColor(70, 70, 70, 255);
	style->Colors[ImGuiCol_TabHovered] = ImColor(76, 76, 76, 255);
	style->Colors[ImGuiCol_TabUnfocused] = ImColor(70, 70, 70, 255);
	style->Colors[ImGuiCol_TabUnfocusedActive] = ImColor(70, 70, 70, 255);

	style->Colors[ImGuiCol_CheckMark] = ImColor(20, 20, 20, 255);
	style->Colors[ImGuiCol_FrameBg] = ImColor(70, 70, 70, 255);

	style->Colors[ImGuiCol_WindowBg] = ImColor(20, 20, 20, 255);

	style->Colors[ImGuiCol_DockingPreview] = ImColor(150, 150, 150, 255);

	guizmoType = ImGuizmo::OPERATION::TRANSLATE;

	std::string assetsPath = "Assets/";
	std::string currentPath = assetsPath;
	std::vector<std::string> dirs;
	std::vector<std::string> dirs2;
	std::vector<std::string> files;
	App->fileSystem->DiscoverFiles(currentPath.c_str(), files, dirs);
	for (int i = 0; i < dirs.size(); i++)
	{
		currentPath = assetsPath + dirs[i] + "/";
		if (currentPath == "Assets/Textures/" || currentPath == "Assets/Models/")
		{
			std::string thisDir = currentPath;
			App->fileSystem->DiscoverFiles(currentPath.c_str(), files, dirs2);
			for (int j = 0; j < files.size(); j++)
			{
				std::string filePath = thisDir + files[j];
				unsigned lastPoint = filePath.find_last_of(".");
				std::string isMeta = filePath.substr(lastPoint, filePath.length() - lastPoint);
				if (isMeta != ".meta")
				{
					u32 auxID = App->resources->ImportFile(filePath.c_str());
				}
			}
			files.clear();
		}
	}

	advanceIcon = App->resources->LoadIcons(App->resources->Find("Assets/Textures/Advance.png"));
	backIcon = App->resources->LoadIcons(App->resources->Find("Assets/Textures/Back.png"));
	folderIcon = App->resources->LoadIcons(App->resources->Find("Assets/Textures/DirectoryIcon.png"));
	fileIcon = App->resources->LoadIcons(App->resources->Find("Assets/Textures/FileIcon.png"));
	pauseIcon = App->resources->LoadIcons(App->resources->Find("Assets/Textures/Pause.png"));
	playIcon = App->resources->LoadIcons(App->resources->Find("Assets/Textures/Play.png"));
	speedDownIcon = App->resources->LoadIcons(App->resources->Find("Assets/Textures/SpeedDown.png"));
	speedUpIcon = App->resources->LoadIcons(App->resources->Find("Assets/Textures/SpeedUp.png"));
	stopIcon = App->resources->LoadIcons(App->resources->Find("Assets/Textures/Stop.png"));

	FBXimporter importer;
	//importer.SpecialreadFromFBX("Assets/Models/Street.fbx", "Library/Models/", nullptr);

	App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, 1, "Game Camera "));
	int lastComponent = App->scene->gameObjects.size() - 1;
	App->editor->objectSelected = App->scene->gameObjects[lastComponent];
	App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent2(ComponentType::CAMERA, float3(0, 0, 0), 75, 1, 20, true));
	App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::TRANSFORM, &float3(0, 0, 0), &float3(1, 1, 1), &float3(0, 0, 0)));
	App->scene->gameObjects[lastComponent]->components[0]->owner = App->scene->gameObjects[lastComponent];
	App->scene->gameObjects[lastComponent]->components[1]->owner = App->scene->gameObjects[lastComponent];
	objectSelected = App->scene->gameObjects[lastComponent];
	App->camera->camArray[0]->isTheMainCamera = true;
	App->camera->GameCam = App->camera->camArray[0];

	isActive = true;
	isActive2 = true;
	isActive3 = true;
	isActive4 = true;
	

	our_font.init("Assets/Fonts/OpenSans-Regular.ttf", 16);
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

	json_serialize_to_file_pretty(user_data, "ConfigFile.json");

	AddLog("Saved Config Data\n");
}

void ModuleEditor::LoadConfig()
{
	if (App->gameMode == false)
	{
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
	}

	if (App->gameMode == true)
	{
		JSON_Value* userData = json_parse_file("GameMode.json");

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
	}

	AddLog("Loaded Config Data\n");
}
int ModuleEditor::loadSpecialObject(int object, const char* direction)
{

	int toReturn = 1;
	JSON_Value* root = json_parse_file(direction);
	JSON_Array* gameObjectsInfo = json_object_dotget_array(json_value_get_object(root), "GameObjects");

	JSON_Value* auxValue = json_array_get_value(gameObjectsInfo, object);

	std::stack<JSON_Value*>stackNode;
	std::stack<GameObject*>parentstack;
	JSON_Value* theObject = json_array_get_value(gameObjectsInfo, object);
	int iteration = 0;

	bool haveChild = true;
	bool toAABB = false;
	GameObject* theObjectParent = new GameObject();
	int newNumber = 0;
	//////////////////////////////////////////////////////


	toAABB = false;
	std::string number = std::to_string(iteration);
	std::string roott = "children";
	std::string name = roott + number;
	std::string nextObjectName = json_object_get_string(json_value_get_object(theObject), name.c_str());

	int numberOfChild = 1;
	numberOfChild = json_object_dotget_number(json_value_get_object(theObject), "Number");
	int numTotal = json_array_get_count(gameObjectsInfo);
	if (numberOfChild > 0)
	{

		for (int y = 0; y < numberOfChild; y++) {
			std::string number = std::to_string(y);
			std::string root = "children";
			std::string name = root + number;
			std::string nextObjectName = json_object_get_string(json_value_get_object(theObject), name.c_str());

			for (int u = 0; u < numTotal; u++)
			{
				JSON_Value* auxSecondValue = json_array_get_value(gameObjectsInfo, u);
				std::string name = json_object_get_string(json_value_get_object(auxSecondValue), "Name");
				if (nextObjectName == name)
				{
					toReturn += 2;
					stackNode.push(auxSecondValue);
					parentstack.push(theObjectParent);
					break;
				}
			}
		}
	}


	haveChild = true;
	while (!stackNode.empty())
	{

		haveChild = true;

		GameObject* aux = App->scene->CreateGameObject(false, 1, "sfe");
		App->editor->objectSelected = aux;
		GameObject* parent = parentstack.top();
		parentstack.pop();
		parent->childrens.push_back(aux);
		//////////////////////////////////////////////////////
		theObject = stackNode.top();
		stackNode.pop();

		int numberOfChild = 0;
		numberOfChild = json_object_dotget_number(json_value_get_object(theObject), "Number");
		if (numberOfChild > 0)
		{
			for (int y = 0; y < numberOfChild; y++) {

				std::string number = std::to_string(y);
				std::string root = "children";
				std::string name = root + number;
				std::string nextObjectName = json_object_get_string(json_value_get_object(theObject), name.c_str());

				for (int u = 0; u < numTotal; u++)
				{
					JSON_Value* auxSecondValue = json_array_get_value(gameObjectsInfo, u);
					std::string name = json_object_get_string(json_value_get_object(auxSecondValue), "Name");
					if (nextObjectName == name)
					{
						toReturn += 2;
						stackNode.push(auxSecondValue);
						parentstack.push(aux);
						break;
					}
				}
			}
		}
		else {
			haveChild = false;
			iteration = 0;
		}





		//////////////////////////////////////////////////////
		aux->name = json_object_get_string(json_value_get_object(theObject), "Name");
		int auxType;



		Component* e;
		auxType = json_object_dotget_number(json_value_get_object(theObject), "Components.Mesh.Type");
		if (auxType == 2) {
			const char* modelPath = json_object_dotget_string(json_value_get_object(theObject), "Components.Mesh.Path");
			FBXimporter importer;
			Mesh* i = importer.readFile(modelPath);
			e = new ComponentMesh(i);
			e->modelPath = modelPath;
			e->name = "mesh Component";
			e->type = ComponentType::MESH;
			aux->components.push_back(e);
			e->owner = aux;
			toAABB = true;
		}
		aux->isImported = true;

		double position_x = json_object_dotget_number(json_value_get_object(theObject), "Components.Transform.Position.x");
		double position_y = json_object_dotget_number(json_value_get_object(theObject), "Components.Transform.Position.y");
		double position_z = json_object_dotget_number(json_value_get_object(theObject), "Components.Transform.Position.z");
		float3 position = float3(position_x, position_y, position_z);
		//Scale
		double scale_x = json_object_dotget_number(json_value_get_object(theObject), "Components.Transform.Scale.x");
		double scale_y = json_object_dotget_number(json_value_get_object(theObject), "Components.Transform.Scale.y");
		double scale_z = json_object_dotget_number(json_value_get_object(theObject), "Components.Transform.Scale.z");
		float3 scale = float3(scale_x, scale_y, scale_z);
		//Rotation
		double rotation_x = json_object_dotget_number(json_value_get_object(theObject), "Components.Transform.Rotation.x");
		double rotation_y = json_object_dotget_number(json_value_get_object(theObject), "Components.Transform.Rotation.y");
		double rotation_z = json_object_dotget_number(json_value_get_object(theObject), "Components.Transform.Rotation.z");
		float3 rotation = float3(rotation_x, rotation_y, rotation_z);
		e = new ComponentTransform(aux, float3(position_x, position_y, position_z), float3(scale_x, scale_y, scale_z), float3(rotation_x, rotation_y, rotation_z));
		e->owner = aux;
		e->type = ComponentType::TRANSFORM;
		aux->components.push_back(e);
		e->setOwner();

		if (toAABB) {
			e->CreateAABB(ComponentType::MESH, aux, true);
			toAABB = false;
		}

		auxType = json_object_dotget_number(json_value_get_object(theObject), "Components.Material.Type");
		if (auxType == 8)
		{
			u32 materialUID = json_object_dotget_number(json_value_get_object(theObject), "Components.Material.UID");
			const char* textPath = json_object_dotget_string(json_value_get_object(theObject), "Components.Material.TexturePath");
			double width = json_object_dotget_number(json_value_get_object(theObject), "Components.Material.Width");
			double height = json_object_dotget_number(json_value_get_object(theObject), "Components.Material.Height");

			u32 materialId = App->resources->RecoveryFile(textPath);
			App->resources->LoadtextureResource(materialId, aux);
			aux->components[aux->components.size() - 1]->owner = aux;
		}
		/*while (!stackNode.empty())
		{
			theObject = stackNode.top();
			stackNode.pop();

			for (unsigned i = 0; i < theObject->childrens.size(); ++i)
			{

				stackNode.push(theObject->childrens[i]);
			}
		}*/
	}
	App->scene->gameObjects.push_back(theObjectParent);
	theObjectParent->isImported = true;
	return toReturn;
}

void ModuleEditor::SaveScene(const char* fileToLoad)
{
	JSON_Value* root = json_value_init_object();
	JSON_Value* arr = json_value_init_array();
	JSON_Object* rootObj = json_value_get_object(root);
	JSON_Array* myArray = json_value_get_array(arr);
	GameObject* go;

	std::stack<GameObject*>stackNode;

	json_object_dotset_value(rootObj, "GameObjects", arr);
	for (int i = 0; i < App->scene->gameObjects.size(); i++)
	{


		for (int a = 0; a < App->scene->gameObjects[i]->childrens.size(); a++)
		{
			stackNode.push(App->scene->gameObjects[i]->childrens[a]);
		}
		stackNode.push(App->scene->gameObjects[i]);
		while (!stackNode.empty())
		{
			go = stackNode.top();
			stackNode.pop();

			//go = App->scene->gameObjects[i];
			JSON_Value* gameObjectValue = json_value_init_object();
			JSON_Object* gameObjObject = json_value_get_object(gameObjectValue);
			json_object_set_number(gameObjObject, "UID", go->UID);
			json_object_set_string(gameObjObject, "Name", go->name.c_str());
			if (go->childrens.size() > 0) {
				json_object_set_string(gameObjObject, "HaveChildren", "YES");
				json_object_dotset_number(gameObjObject, "Number", go->childrens.size());
			}
			else {
				json_object_set_string(gameObjObject, "HaveChildren", "NO");
			}
			for (int p = 0; p < go->childrens.size(); p++) {
				std::string name = "children";

				std::string theNumber;
				theNumber = std::to_string(p);
				name = name + theNumber;
				json_object_set_string(gameObjObject, name.c_str(), go->childrens[p]->name.c_str());
			}
			for (int j = 0; j < go->components.size(); j++)
			{
				if (go->components[j]->type == ComponentType::MESH)
				{
					json_object_dotset_number(gameObjObject, "Components.Mesh.Type", (double)go->components[j]->type);
					json_object_dotset_number(gameObjObject, "Components.Mesh.UID", go->components[j]->UID);
					json_object_dotset_string(gameObjObject, "Components.Mesh.Path", go->components[j]->modelPath.c_str());
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
			for (unsigned i = 0; i < go->childrens.size(); ++i)
			{
				stackNode.push(go->childrens[i]);
			}
		}
	}

	json_serialize_to_file_pretty(root, fileToLoad);
	AddLog("Saved Scene Data\n");
}

void ModuleEditor::LoadScene(const char* fileToLoad)
{
	App->scene->gameObjects.clear();
	for (auto it = App->resources->resources.begin(); it != App->resources->resources.end(); it++)
	{
		if (it->second->libraryFile == std::string(fileToLoad))
		{
			it->second->UnloadFromMemory();
			App->resources->ReleaseResource(it->first);
		}
	}

	JSON_Value* root = json_parse_file(fileToLoad);
	JSON_Array* gameObjectsInfo = json_object_dotget_array(json_value_get_object(root), "GameObjects");
	int numOfIterations = json_array_get_count(gameObjectsInfo);

	for (size_t i = 0; i < numOfIterations; i++)
	{
		JSON_Value* auxValue = json_array_get_value(gameObjectsInfo, i);
		std::string meshpath = "NOT RESULT";
		meshpath = json_object_get_string(json_value_get_object(auxValue), "HaveChildren");

		if (meshpath == "YES")
		{

			int iter = loadSpecialObject(i, fileToLoad);
			i = iter + i - 1;
		}
		else 
		{
			//do normal
			const char* name = json_object_get_string(json_value_get_object(auxValue), "Name");
			u32 UID = json_object_get_number(json_value_get_object(auxValue), "UID");

			std::string futureNumber = std::string(name);
			size_t start = futureNumber.find_last_of("_");
			futureNumber = futureNumber.substr(start + 1, futureNumber.length() - start);
			App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, atoi(futureNumber.c_str()), name)); // Create GO with name
			objectSelected = App->scene->gameObjects[App->scene->gameObjects.size() - 1];
			//objectSelected = App->scene->gameObjects[App->scene->gameObjects.size() - 1];
			GameObject* lastGO = objectSelected;
			lastGO->UID = UID; //UID

			std::string isEmpty = std::string(name);
			isEmpty = isEmpty.substr(0, 5);
			if (isEmpty == "Empty")
			{
				App->scene->gameObjects[App->scene->gameObjects.size() - 1]->id = lastId + 1;
				lastId++;
			}

			/*  MESH   */
			int auxType = json_object_dotget_number(json_value_get_object(auxValue), "Components.Mesh.Type");
			u32 compUID = json_object_dotget_number(json_value_get_object(auxValue), "Components.Mesh.UID");
			bool wire = json_object_dotget_boolean(json_value_get_object(auxValue), "Components.Mesh.Wireframe");
			bool act = json_object_dotget_boolean(json_value_get_object(auxValue), "Components.Mesh.Active");

			if (auxType == 2)
			{
				bool nor = json_object_dotget_boolean(json_value_get_object(auxValue), "Components.Mesh.Normals");
				const char* modelPath = json_object_dotget_string(json_value_get_object(auxValue), "Components.Mesh.Path");

				u32 modelId = App->resources->RecoveryFile(modelPath);
				App->resources->LoadResource(modelId, lastGO);
				lastGO->components[lastGO->components.size() - 1]->owner = lastGO;
			}
			if (auxType == 3)
			{
				lastGO->components.push_back(lastGO->CreateComponent(ComponentType::CUBE, &float3(0, 0, 0), &float3(1, 1, 1), &float3(0, 0, 0)));
				lastGO->components[lastGO->components.size() - 1]->owner = lastGO;
			}
			if (auxType == 6)
			{
				lastGO->components.push_back(lastGO->CreateComponent(ComponentType::CYLINDER, &float3(0, 0, 0), &float3(1, 1, 1), &float3(0, 0, 0)));
				lastGO->components[lastGO->components.size() - 1]->owner = lastGO;
			}
			if (auxType == 7)
			{
				lastGO->components.push_back(lastGO->CreateComponent(ComponentType::PLANE, &float3(0, 0, 0), &float3(1, 1, 1), &float3(0, 0, 0)));
				lastGO->components[lastGO->components.size() - 1]->owner = lastGO;
			}
			if (auxType == 4)
			{
				lastGO->components.push_back(lastGO->CreateComponent(ComponentType::PYRAMID, &float3(0, 0, 0), &float3(1, 1, 1), &float3(0, 0, 0)));
				lastGO->components[lastGO->components.size() - 1]->owner = lastGO;
			}
			if (auxType == 5)
			{
				lastGO->components.push_back(lastGO->CreateComponent(ComponentType::SPHERE, &float3(0, 0, 0), &float3(1, 1, 1), &float3(0, 0, 0)));
				lastGO->components[lastGO->components.size() - 1]->owner = lastGO;
			}

			/*   CAMERAS   */
			auxType = json_object_dotget_number(json_value_get_object(auxValue), "Components.Camera.Type");
			if (auxType == 9)
			{
				u32 cameraUID = json_object_dotget_number(json_value_get_object(auxValue), "Components.Camera.UID");
				int Type = json_object_dotget_number(json_value_get_object(auxValue), "Components.Camera.Type");
				double horizontalFov = json_object_dotget_number(json_value_get_object(auxValue), "Components.Camera.FOV");
				double nearPlane = json_object_dotget_number(json_value_get_object(auxValue), "Components.Camera.NearPlane");
				double farPlane = json_object_dotget_number(json_value_get_object(auxValue), "Components.Camera.FarPlane");
				lastGO->components.push_back(lastGO->CreateComponent2(ComponentType::CAMERA, position, horizontalFov, nearPlane, farPlane, true));
			}

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
			lastGO->components[lastGO->components.size() - 1]->owner = lastGO;

			/*   MATERIALS   */
			auxType = json_object_dotget_number(json_value_get_object(auxValue), "Components.Material.Type");
			if (auxType == 8)
			{
				u32 materialUID = json_object_dotget_number(json_value_get_object(auxValue), "Components.Material.UID");
				const char* textPath = json_object_dotget_string(json_value_get_object(auxValue), "Components.Material.TexturePath");
				double width = json_object_dotget_number(json_value_get_object(auxValue), "Components.Material.Width");
				double height = json_object_dotget_number(json_value_get_object(auxValue), "Components.Material.Height");

				u32 materialId = App->resources->RecoveryFile(textPath);
				App->resources->LoadResource(materialId, lastGO);
				lastGO->components[lastGO->components.size() - 1]->owner = lastGO;
			}
		}
	}
	UpdateAll();
	//load = true;
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
	MyCube* auxCube;
	auxCube = new MyCube(pos, sca);
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
void ModuleEditor::DrawPrimitives()
{

	std::stack<GameObject*>stackNode;
	GameObject* theObject;
	for (int i = 0; i < App->scene->gameObjects.size(); i++)
	{

		for (int a = 0; a < App->scene->gameObjects[i]->childrens.size(); a++) {
			stackNode.push(App->scene->gameObjects[i]->childrens[a]);
		}
		while (!stackNode.empty())
		{
			theObject = stackNode.top();
			stackNode.pop();
			/*int y = App->scene->gameObjects[i]->aabb.size();
			math::AABB* e = theObject->aabb[y - 1];*/
			
			if (showAABB)
			{
				for (int a = 0; a < theObject->boundingBoxes.size(); a++)
				{
					theObject->boundingBoxes[a]->DrawCube();
				}
			}
					
			for (unsigned i = 0; i < theObject->childrens.size(); ++i)
			{
				stackNode.push(theObject->childrens[i]);
			}
		}
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
			//Pyramids
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
	auxCylinder = new MyCylinder(pos, sca);
	cylinders.push_back(auxCylinder);
}

void ModuleEditor::DOptionsmenu(ComponentType type)
{
	int lastComponent = 0;

	switch (type)
	{
	case ComponentType::CUBE:

			if (cubes.size() == 0)
				App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, 1, "Cube "));
			else
				App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, cubes.size() + 1, "Cube "));

			lastComponent = App->scene->gameObjects.size() - 1;
			objectSelected = App->scene->gameObjects[lastComponent];
			App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::CUBE, &position, &M, &float3(0, 0, 0)));
			App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::TRANSFORM, &position, &M, &float3(0, 0, 0)));
			App->scene->gameObjects[lastComponent]->components[0]->owner = App->scene->gameObjects[lastComponent];
			App->scene->gameObjects[lastComponent]->components[1]->owner = App->scene->gameObjects[lastComponent];

			showOptionsMenu = ComponentType::NONE;
			App->editor->AddLog("Cube Created\n");
		break;

	case ComponentType::PYRAMID:

			if (pyramids.size() == 0)
				App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, 1, "Pyramid "));
			else
				App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, pyramids.size() + 1, "Pyramid "));

			lastComponent = App->scene->gameObjects.size() - 1;
			objectSelected = App->scene->gameObjects[lastComponent];
			App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::PYRAMID, &position, &M, &float3(0, 0, 0)));
			App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::TRANSFORM, &position, &M, &float3(0, 0, 0)));
			App->scene->gameObjects[lastComponent]->components[0]->owner = App->scene->gameObjects[lastComponent];
			App->scene->gameObjects[lastComponent]->components[1]->owner = App->scene->gameObjects[lastComponent];
			showOptionsMenu = ComponentType::NONE;

			App->editor->AddLog("Pyramid Created\n");
		break;

	case ComponentType::CYLINDER:

			if (cylinders.size() == 0)
				App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, 1, "Cylinder "));
			else
				App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, cylinders.size() + 1, "Cylinder "));

			lastComponent = App->scene->gameObjects.size() - 1;
			objectSelected = App->scene->gameObjects[lastComponent];
			App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::CYLINDER, &position, &M, &float3(0, 0, 0)));
			App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::TRANSFORM, &position, &M, &float3(0, 0, 0)));
			App->scene->gameObjects[lastComponent]->components[0]->owner = App->scene->gameObjects[lastComponent];
			App->scene->gameObjects[lastComponent]->components[1]->owner = App->scene->gameObjects[lastComponent];
			showOptionsMenu = ComponentType::NONE;

			App->editor->AddLog("Cylinder Created\n");
		break;

	case ComponentType::SPHERE:

			if (spheres.size() == 0)
				App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, 1, "Sphere "));
			else
				App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, spheres.size() + 1, "Sphere "));

			lastComponent = App->scene->gameObjects.size() - 1;
			objectSelected = App->scene->gameObjects[lastComponent];
			App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::SPHERE, &position, &M, &float3(0, 0, 0)));
			App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::TRANSFORM, &position, &M, &float3(0, 0, 0)));
			App->scene->gameObjects[lastComponent]->components[0]->owner = App->scene->gameObjects[lastComponent];
			App->scene->gameObjects[lastComponent]->components[1]->owner = App->scene->gameObjects[lastComponent];
			showOptionsMenu = ComponentType::NONE;

			App->editor->AddLog("Sphere Created\n");
		break;

	case ComponentType::PLANE:

			if (planes.size() == 0)
				App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, 1, "Plane "));
			else
				App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, planes.size() + 1, "Plane "));

			lastComponent = App->scene->gameObjects.size() - 1;
			objectSelected = App->scene->gameObjects[lastComponent];
			App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::PLANE, &position, &M, &float3(0, 0, 0)));
			App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::TRANSFORM, &position, &M, &float3(0, 0, 0)));
			App->scene->gameObjects[lastComponent]->components[0]->owner = App->scene->gameObjects[lastComponent];
			App->scene->gameObjects[lastComponent]->components[1]->owner = App->scene->gameObjects[lastComponent];
			showOptionsMenu = ComponentType::NONE;

			App->editor->AddLog("Plane Created\n");
		break;
	default:
		break;
	}
}
// Update: draw background
update_status ModuleEditor::Update(float dt)
{

	if (App->camera->camArray.size() == 0) {
		App->camera->GameCam = nullptr;
	}
	if (App->camera->GameCam != nullptr) {
		App->camera->GameCam->Update();
	}
	App->camera->editorCam->Update();


	// Change Guizmo types
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		guizmoType = ImGuizmo::OPERATION::TRANSLATE;
	}
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		guizmoType = ImGuizmo::OPERATION::ROTATE;
	}
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		guizmoType = ImGuizmo::OPERATION::SCALE;
	}

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		objectSelected = nullptr;
	}

	if (DockingRootItem("Viewport", ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
	}

	if (objectSelected != nullptr && App->input->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN)
	{
		if (objectSelected->name == "Game Camera _1" || objectSelected->name == "Game Camera _1_1")
		{
			App->editor->AddLog("You can't delete the Game Camera\n");
		}
		else
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
					delete (*(App->scene->gameObjects.begin() + i));
					App->scene->gameObjects.erase(App->scene->gameObjects.begin() + i);

					break;
				}
			}
		}
	}

	static bool showDemoWindow;
	static bool showAnotherWindow;
	static bool showCloseWindow;
	static bool showAboutWindow;
	static bool openConfigMenu;
	static bool openSceneMenu;
	static bool showConfigMenu;
	static bool showWarningMenu;
	static bool showHierarchy;
	static bool showInspector;
	static bool showConsoleMenu;
	static bool showSceneWindow;
	static bool showScene2Window;
	static bool showTimeWindow;
	static bool showOptions;
	static bool showAssets;
	static bool showTextureMenu;
	static bool showMeshMenu;

	// Our state
	if (App->gameMode == false) {

		showAnotherWindow = false;
		showCloseWindow = true;
		openConfigMenu = false;
		openSceneMenu = false;
		showConfigMenu = true;
		showHierarchy = true;
		showInspector = true;
		showConsoleMenu = true;
		showSceneWindow = true;
		showScene2Window = true;
		showTimeWindow = true;
		showOptions = true;
		showAssets = true;
	}

	if (App->gameMode == true) {
		showDemoWindow = false;
		showAnotherWindow = false;
		showCloseWindow = true;
		showAboutWindow = false;
		openConfigMenu = false;
		openSceneMenu = false;
		showConfigMenu = false;
		showWarningMenu = false;
		showHierarchy = false;
		showInspector = false;
		showConsoleMenu = false;
		showSceneWindow = false;
		showScene2Window = true;
		showTimeWindow = true;
		showOptions = true;
		showTextureMenu = false;
		showMeshMenu = false;
		isActive = true;
		isActive2 = true;
		isActive3 = true;
		isActive4 = true;
		showAssets = false;
	}
	ImVec4 clearColor = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

	//////////////////////////////////////////////////////////////////////////////////////////// SHAPES BEGIN OPTIONS ////////////////////////////////////////////////////////////////////////////////////////////
	if (showOptionsMenu != ComponentType::NONE) {
		DOptionsmenu(showOptionsMenu);

	}
	//////////////////////////////////////////////////////////////////////////////////////////// MAIN MENU BAR ////////////////////////////////////////////////////////////////////////////////////////////
	if (App->gameMode == false)
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File", &openConfigMenu))
			{
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
					FileDialog fileDialog;
					std::string fileToLoad = fileDialog.SaveScene("Chaos Scene (*.chaos)\0*.chaos\0");
					SaveScene(fileToLoad.c_str());
				}
				if (ImGui::MenuItem("Load Scene"))
				{
					showWarningMenu = !showWarningMenu;
				}
				ImGui::EndMenu();
			}


			if (ImGui::BeginMenu("View", &openConfigMenu))
			{
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
				if (ImGui::MenuItem("Enable/Disable Raycast"))
				{
					showRay = !showRay;
					if (showRay)
					{
						App->editor->AddLog("Raycast Enabled\n");
					}
					else
					{
						App->editor->AddLog("Raycast Disabled\n");
					}
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("GameObject", &openConfigMenu))
			{

				if (ImGui::MenuItem("Create Camera"))
				{
					static int cameras = 1;
					App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, cameras, "Camera "));
					cameras++;
					int lastComponent = App->scene->gameObjects.size() - 1;
					App->editor->objectSelected = App->scene->gameObjects[lastComponent];
					App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent2(ComponentType::CAMERA, float3(0, 0, 0), 75, 1, 20, true));
					App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::TRANSFORM, &float3(0, 0, 0), &float3(1, 1, 1), &float3(0, 0, 0)));
					App->scene->gameObjects[lastComponent]->components[0]->owner = App->scene->gameObjects[lastComponent];
					App->scene->gameObjects[lastComponent]->components[1]->owner = App->scene->gameObjects[lastComponent];
					objectSelected = App->scene->gameObjects[lastComponent];
					App->editor->AddLog("Camera Object Created\n");
				}
				if (ImGui::MenuItem("Create Empty"))
				{
					static int empties = 1;
					App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, empties, "Empty "));
					empties++;
					int lastComponent = App->scene->gameObjects.size() - 1;
					App->scene->gameObjects[App->scene->gameObjects.size() - 1]->id = lastId + 1;
					lastId++;
					App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::TRANSFORM, &float3(0, 0, 0), &float3(1, 1, 1), &float3(0, 0, 0)));
					App->scene->gameObjects[lastComponent]->components[0]->owner = App->scene->gameObjects[lastComponent];
					objectSelected = App->scene->gameObjects[lastComponent];
					App->editor->AddLog("Empty Object Created\n");
				}
				if (ImGui::MenuItem("Create House"))
				{
					static int houses = 1;
					App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, houses, "House "));
					houses++;
					int lastComponent = App->scene->gameObjects.size() - 1;
					objectSelected = App->scene->gameObjects[lastComponent];

					App->resources->LoadResource(App->resources->Find("Assets/Models/BakerHouse.fbx"),App->scene->gameObjects[lastComponent]);
					objectSelected->components[0]->owner = objectSelected;
					objectSelected->components.push_back(objectSelected->CreateComponent(ComponentType::TRANSFORM, &float3(0, 0, 0), &float3(1, 1, 1), &float3(0, 0, 0)));
					objectSelected->components[1]->owner = objectSelected;
					App->resources->LoadResource(App->resources->Find("Assets/Textures/BakerHouse.png"), App->scene->gameObjects[lastComponent]);
					objectSelected->components[2]->owner = objectSelected;

					App->editor->AddLog("House Created\n");
				}
				if (ImGui::MenuItem("Create Penguin"))
				{
					static int penguins = 1;
					App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, penguins, "Penguin "));
					penguins++;
					int lastComponent = App->scene->gameObjects.size() - 1;
					objectSelected = App->scene->gameObjects[lastComponent];

					App->resources->LoadResource(App->resources->Find("Assets/Models/Penguin.fbx"), App->scene->gameObjects[lastComponent]);
					objectSelected->components[0]->owner = objectSelected;
					objectSelected->components.push_back(objectSelected->CreateComponent(ComponentType::TRANSFORM, &float3(0, 0, 0), &float3(1, 1, 1), &float3(0, 0, 0)));
					objectSelected->components[1]->owner = objectSelected;
					App->resources->LoadResource(App->resources->Find("Assets/Textures/Penguin.png"), App->scene->gameObjects[lastComponent]);
					objectSelected->components[2]->owner = objectSelected;

					App->editor->AddLog("Penguin Created\n");
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
				ImGui::Separator();
				if (ImGui::MenuItem("Create Button"))
				{
					static int buttons = 1;
					App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, buttons, "Button "));
					buttons++;
					int lastComponent = App->scene->gameObjects.size() - 1;
					objectSelected = App->scene->gameObjects[lastComponent];

					objectSelected->components.push_back(objectSelected->CreateComponent(ComponentType::TRANSFORM2D, &float3(0, 0, 0), &float3(1.5f, 1, 0.5f), &float3(90, 0, 0)));
					objectSelected->components[0]->owner = objectSelected;
					objectSelected->components.push_back(objectSelected->CreateUIComponent(ComponentType::UI_BUTTON));
					objectSelected->components[1]->owner = objectSelected;
					objectSelected->components.push_back(objectSelected->CreateComponent(ComponentType::PLANE, &float3(0, 0, 0), &float3(1, 1, 1), &float3(0, 0, 0)));
					objectSelected->components[2]->owner = objectSelected;

					App->editor->AddLog("Button Created\n");
				}
				if (ImGui::MenuItem("Create Image"))
				{
					static int images = 1;
					App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, images, "Image "));
					images++;
					int lastComponent = App->scene->gameObjects.size() - 1;
					objectSelected = App->scene->gameObjects[lastComponent];

					objectSelected->components.push_back(objectSelected->CreateComponent(ComponentType::TRANSFORM2D, &float3(0, 0, 0), &float3(1, 1, 1), &float3(0, 0, 0)));
					objectSelected->components[0]->owner = objectSelected;
					objectSelected->components.push_back(objectSelected->CreateUIComponent(ComponentType::UI_IMAGE));
					objectSelected->components[1]->owner = objectSelected;

					App->editor->AddLog("Image Created\n");
				}
				if (ImGui::MenuItem("Create Check Box"))
				{
					static int checkboxes = 1;
					App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, checkboxes, "Check Box "));
					checkboxes++;
					int lastComponent = App->scene->gameObjects.size() - 1;
					objectSelected = App->scene->gameObjects[lastComponent];

					objectSelected->components.push_back(objectSelected->CreateComponent(ComponentType::TRANSFORM2D, &float3(0, 0, 0), &float3(1, 1, 1), &float3(0, 0, 0)));
					objectSelected->components[0]->owner = objectSelected;
					objectSelected->components.push_back(objectSelected->CreateUIComponent(ComponentType::UI_CHECKBOX));
					objectSelected->components[1]->owner = objectSelected;

					App->editor->AddLog("Check Box Created\n");
				}
				if (ImGui::MenuItem("Create Slider"))
				{
					static int sliders = 1;
					App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, sliders, "Slider "));
					sliders++;
					int lastComponent = App->scene->gameObjects.size() - 1;
					objectSelected = App->scene->gameObjects[lastComponent];

					objectSelected->components.push_back(objectSelected->CreateComponent(ComponentType::TRANSFORM2D, &float3(0, 0, 0), &float3(1, 1, 1), &float3(0, 0, 0)));
					objectSelected->components[0]->owner = objectSelected;
					objectSelected->components.push_back(objectSelected->CreateUIComponent(ComponentType::UI_SLIDER));
					objectSelected->components[1]->owner = objectSelected;

					App->editor->AddLog("Slider Created\n");
				}
				if (ImGui::MenuItem("Create Input Box"))
				{
					static int inputboxes = 1;
					App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, inputboxes, "Input Box "));
					inputboxes++;
					int lastComponent = App->scene->gameObjects.size() - 1;
					objectSelected = App->scene->gameObjects[lastComponent];

					objectSelected->components.push_back(objectSelected->CreateComponent(ComponentType::TRANSFORM2D, &float3(0, 0, 0), &float3(1, 1, 1), &float3(0, 0, 0)));
					objectSelected->components[0]->owner = objectSelected;
					objectSelected->components.push_back(objectSelected->CreateUIComponent(ComponentType::UI_INPUTBOX));
					objectSelected->components[1]->owner = objectSelected;

					App->editor->AddLog("Input Box Created\n");
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

			if (ImGui::BeginMenu("Quit", &openConfigMenu))
			{
				return update_status::UPDATE_STOP;
			}
			ImGui::EndMainMenuBar();
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////// TEXTURE WINDOW ////////////////////////////////////////////////////////////////////////////////////////////

	///FILTERS
	static bool mipMap;

	static bool alienfy;
	static bool contrast;
	static bool negativity;
	static bool sharpening;
	static bool gaussianBlur;
	static bool equalization;
	static bool noise;
	static bool averageBlur;
	static bool gammaCorrection;
	static bool pixelization;
	static float contrastAmount;
	static float sharpenFactor;
	static float sharpenIters;
	static float avBlurAmount;
	static float gaussianBlurAmount;
	static float gammaCorrectionAmount;
	static float noiseAmount;
	static float pixelationAmount;
	static std::string compression;
	static bool selected[6];

	if (showTextureMenu)
	{
		ImGui::CloseCurrentPopup();
		ImGui::Begin("Texture Import Settings", &showTextureMenu, ImGuiWindowFlags_NoScrollbar);
		
		const char* items[]{ "IL_DXT_NO_COMP","IL_DXT1" ,"IL_DXT2" ,"IL_DXT3" ,"IL_DXT4" ,"IL_DXT5" };

		ResourceMatertial* res = (ResourceMatertial*)newResource;

		static std::string previewValue = " ";
		// Select Compression Type
		if (ImGui::BeginCombo("Compression",previewValue.c_str()))
		{
			previewValue = " ";
			for (int i = 0; i < IM_ARRAYSIZE(items); i++)
			{
				if (ImGui::Selectable(items[i],&selected[i]))
				{
					compression = items[i];
					previewValue = compression;
				}
			}
			ImGui::EndCombo();
		}

		if (!res->metaData.isLoaded)
		{
			mipMap = res->metaData.mipMap;
			alienfy = res->metaData.alienifying;
			averageBlur = res->metaData.avgBlurring;
			contrast = res->metaData.contrast;
			negativity = res->metaData.negativity;
			sharpening = res->metaData.sharpering;
			gaussianBlur = res->metaData.gausianBlurring; 
			equalization = res->metaData.equalization;
		    noise = res->metaData.noise;
			gammaCorrection = res->metaData.gammaCorrection;
			pixelization = res->metaData.pixelization;

			avBlurAmount = res->metaData.amountAvBlur;
			contrastAmount = res->metaData.amountContrast;
			pixelationAmount = res->metaData.amountGammaCorrection;
			gaussianBlurAmount = res->metaData.amountGausianBlur;
			noiseAmount = res->metaData.amountNoise;
			pixelationAmount = res->metaData.amountPixelation;
			sharpenIters = res->metaData.sharpenIters;
			sharpenFactor = res->metaData.sharpenFactor;
			compression = res->metaData.compression;

			res->metaData.isLoaded = true;
		}

		if (ImGui::CollapsingHeader("Settings"))
		{
			if (ImGui::Checkbox("Mip-Map", &mipMap))
			{
				res->metaData.mipMap = mipMap;
			}
		}

		ImGui::Separator();
		if (ImGui::CollapsingHeader("Filters"))
		{
			static float padding = 1.0f;
			static float thumbnailSize = 128.0f;
			float cellSize = thumbnailSize + padding;

			float panelWidth = ImGui::GetContentRegionAvail().x;
			int columnCount = (int)(panelWidth / cellSize);
			if (columnCount < 1)
				columnCount = 1;
			ImGui::Columns(columnCount, 0, false);
			if (ImGui::Checkbox("Alienfy", &alienfy))
			{
				res->metaData.alienifying = alienfy;
			}
			ImGui::NextColumn();
			if (ImGui::Checkbox("Contrast", &contrast))
			{
				res->metaData.contrast = contrast;
			}
			ImGui::NextColumn();
			if (ImGui::Checkbox("Negativity", &negativity))
			{
				res->metaData.negativity = negativity;
			}
			ImGui::NextColumn();
			if (ImGui::Checkbox("Sharpening", &sharpening))
			{
				res->metaData.sharpering = sharpening;
			}
			ImGui::NextColumn();
			if (ImGui::Checkbox("Gaussian Blur", &gaussianBlur))
			{
				res->metaData.gausianBlurring = gaussianBlur;
			}
			ImGui::NextColumn();
			if (ImGui::Checkbox("Equalization", &equalization))
			{
				res->metaData.equalization = equalization;
			}
			ImGui::NextColumn();
			if (ImGui::Checkbox("Noise", &noise))
			{
				res->metaData.noise = noise;
			}
			ImGui::NextColumn();
			if (ImGui::Checkbox("Average Blur", &averageBlur))
			{
				res->metaData.avgBlurring = averageBlur;
			}
			ImGui::NextColumn();
			if (ImGui::Checkbox("Gamma Correction", &gammaCorrection))
			{
				res->metaData.gammaCorrection = gammaCorrection;
			}
			ImGui::NextColumn();
			if (ImGui::Checkbox("Pixelization", &pixelization))
			{
				res->metaData.pixelization = pixelization;
			}
			ImGui::NextColumn();
			ImGui::Text("Amount of Pixelization:");
			if (ImGui::DragFloat("", &pixelationAmount, 0.1f, 1, 10));
			{
				res->metaData.amountPixelation = pixelationAmount;
			}
			ImGui::NextColumn();
			ImGui::Text("Amount of Contrast:");
			if (ImGui::DragFloat(" ", &contrastAmount, 0.1f, -2, 2));
			{
				res->metaData.amountContrast = contrastAmount;
			}
			ImGui::NextColumn();
			ImGui::Text("Sharpering Factor:");
			if (ImGui::DragFloat("  ", &sharpenFactor, 0.1f, 0, 2.5));
			{
				res->metaData.sharpenFactor = sharpenFactor;
			}
			ImGui::NextColumn();
			ImGui::Text("Sharpering Iterations:");
			if (ImGui::DragFloat("   ", &sharpenIters, 0.1f, 0, 5));
			{
				res->metaData.sharpenIters = sharpenIters;
			}
			ImGui::NextColumn();
			ImGui::Text("Amount of Gaussian Blur:");
			if (ImGui::DragFloat("    ", &gaussianBlurAmount, 0.1f, 1, 10));
			{
				res->metaData.amountGausianBlur = gaussianBlurAmount;
			}
			ImGui::NextColumn();
			ImGui::Text("Amount of Noise:");
			if (ImGui::DragFloat("     ", &noiseAmount, 0.1f, 0, 1));
			{
				res->metaData.amountNoise = noiseAmount;
			}
			ImGui::NextColumn();
			ImGui::Text("Amount of Average Blur:");
			if (ImGui::DragFloat("       ", &avBlurAmount, 0.1f, 1, 10));
			{
				res->metaData.amountAvBlur = avBlurAmount;
			}
			ImGui::NextColumn();
			ImGui::Text("Amount of Gamma Correction:");
			if (ImGui::DragFloat("        ", & gammaCorrectionAmount, 0.1f, 0, 2));
			{
				res->metaData.amountGammaCorrection = gammaCorrectionAmount;
			}
			ImGui::NextColumn();
		}

		ImGui::Separator();
		if (ImGui::Button("Apply and Import"))
		{
			int oldMaterialId;
			oldMaterialId = App->editor->objectSelected->SearchComponent(App->editor->objectSelected, ComponentType::MATERIAL);
			if (oldMaterialId != -1)
			{
				objectSelected->components.erase(objectSelected->components.begin() + oldMaterialId);
			}
			ResourceMatertial* res = (ResourceMatertial*)newResource;
			res->GenerateMeta(mipMap,alienfy, averageBlur, gaussianBlur, contrast, equalization, gammaCorrection, negativity, noise, pixelization, sharpening, contrastAmount, sharpenFactor, sharpenIters, avBlurAmount, gaussianBlurAmount, gammaCorrectionAmount, noiseAmount, pixelationAmount,compression);
			App->resources->LoadResource(newResource->GetUID(), objectSelected);
			objectSelected->components[objectSelected->components.size() - 1]->owner = objectSelected;
			droppedTexture = (ImTextureID)newResource->GetTextureId();
			showTextureMenu = false;
		}
		if (ImGui::Button("Set Default"))
		{
			res->metaData.mipMap = mipMap = false;
			res->metaData.alienifying = alienfy = false;
			res->metaData.avgBlurring = averageBlur = false;
			res->metaData.contrast = contrast = false;
			res->metaData.negativity = negativity = false;
			res->metaData.sharpering = sharpening = false;
			res->metaData.gausianBlurring = gaussianBlur = false;
			res->metaData.equalization = equalization = false;
			res->metaData.noise = noise = false;
			res->metaData.gammaCorrection = gammaCorrection = false;
			res->metaData.pixelization = pixelization = false;

			res->metaData.amountAvBlur = avBlurAmount = 10;
			res->metaData.amountContrast = contrastAmount = 1.6;
			res->metaData.amountGammaCorrection = pixelationAmount = 10;
			res->metaData.amountGausianBlur = gaussianBlurAmount = 10;
			res->metaData.amountNoise = noiseAmount = 0.5;
			res->metaData.amountPixelation = pixelationAmount = 10;
			res->metaData.sharpenIters = sharpenIters = 5;
			res->metaData.sharpenFactor = sharpenFactor = 1.5;
			res->metaData.compression = compression = "IL_DXT5";

			App->resources->LoadResource(newResource->GetUID(),objectSelected);
			droppedTexture = (ImTextureID)newResource->GetTextureId();
		}

		ImGui::Image((void*)(intptr_t)droppedTexture, ImVec2(200, 200));

		ImGui::End();
	}

	//////////////////////////////////////////////////////////////////////////////////////////// MESH WINDOW ////////////////////////////////////////////////////////////////////////////////////////////

	//Options
	static bool joinVertex;
	static bool triangulate;
	static bool generateNormals;
	static bool generateSmoothNormals;
	static bool removeMaterials;
	static bool infacingNormals;
	static bool generateUvsCoords;
	static bool transformUvsCoords;
	static bool findInstances;
	static bool optimizeMesh;
	static bool flipUvs;

	if (showMeshMenu)
	{
		ResourceMesh* res = (ResourceMesh*)newResource;
		ImGui::CloseCurrentPopup();
		ImGui::Begin("Mesh Import Settings", &showMeshMenu, ImGuiWindowFlags_NoScrollbar);

		if (ImGui::CollapsingHeader("Settings"))
		{
			static float padding = 1.0f;
			static float thumbnailSize = 128.0f;
			float cellSize = thumbnailSize + padding;
			float panelWidth = ImGui::GetContentRegionAvail().x;
			int columnCount = (int)(panelWidth / cellSize);
			if (columnCount < 1)
				columnCount = 1;

			if (!res->metaData.isLoaded)
			{
				joinVertex = res->metaData.joinVertex;
				triangulate = res->metaData.triangulate;
				generateNormals = res->metaData.generateNormals;
				generateSmoothNormals = res->metaData.generateSmoothNormals;
				removeMaterials = res->metaData.removeMaterials;
				infacingNormals = res->metaData.infacingNormals;
				generateUvsCoords = res->metaData.genUvCoords;
				transformUvsCoords = res->metaData.transUvCoords;
				findInstances = res->metaData.findInstances;
				optimizeMesh = res->metaData.optimizeMesh;
				flipUvs = res->metaData.flipUvs;

				res->metaData.isLoaded = true;
			}

			ImGui::Columns(columnCount, 0, false);

			if (ImGui::Checkbox("Join Vertex", &joinVertex))
			{
				res->metaData.joinVertex = joinVertex;
			}
			ImGui::NextColumn();
			if (ImGui::Checkbox("Triangulate", &triangulate))
			{
				res->metaData.triangulate = triangulate;
			}
			ImGui::NextColumn();
			if (ImGui::Checkbox("Generate Normals", &generateNormals))
			{
				res->metaData.generateNormals = generateNormals;
			}
			ImGui::NextColumn();
			if (ImGui::Checkbox("Generate Smooth Normals", &generateSmoothNormals))
			{
				res->metaData.generateSmoothNormals = generateSmoothNormals;
			}
			ImGui::NextColumn();
			if (ImGui::Checkbox("Remove Materials", &removeMaterials))
			{
				res->metaData.removeMaterials = removeMaterials;
			}
			ImGui::NextColumn();
			if (ImGui::Checkbox("Infacing Normals", &infacingNormals))
			{
				res->metaData.infacingNormals = infacingNormals;
			}
			ImGui::NextColumn();
			if (ImGui::Checkbox("Generate UVs Coords", &generateUvsCoords))
			{
				res->metaData.genUvCoords = generateUvsCoords;
			}
			ImGui::NextColumn();
			if (ImGui::Checkbox("Transform UVs Coords", &transformUvsCoords))
			{
				res->metaData.transUvCoords = transformUvsCoords;
			}
			ImGui::NextColumn();
			if (ImGui::Checkbox("Find Instances", &findInstances))
			{
				res->metaData.findInstances = findInstances;
			}
			ImGui::NextColumn();
			if (ImGui::Checkbox("Optimize Mesh", &optimizeMesh))
			{
				res->metaData.optimizeMesh = optimizeMesh;
			}
			ImGui::NextColumn();
			if (ImGui::Checkbox("Flip UVs", &flipUvs))
			{
				res->metaData.flipUvs = flipUvs;
			}
			ImGui::NextColumn();
		}

		if (ImGui::Button("Import"))
		{
			showMeshMenu = !showMeshMenu;

			ResourceMesh* res = (ResourceMesh*)newResource;
			res->GenerateMeta(joinVertex,triangulate,generateNormals,generateSmoothNormals,removeMaterials,infacingNormals,generateUvsCoords,transformUvsCoords,findInstances,optimizeMesh,flipUvs);
			static uint importedGobjs = 1;
			App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, importedGobjs, "Game Object"));
			importedGobjs++;
			u32 newResID = App->resources->ImportFile(newResource->GetAssetFile());
			objectSelected = App->scene->gameObjects[App->scene->gameObjects.size() - 1];
			App->resources->LoadResource(newResID, App->scene->gameObjects[App->scene->gameObjects.size() - 1]);
			objectSelected->components[objectSelected->components.size() - 1]->owner = objectSelected;
			objectSelected->components.push_back(objectSelected->CreateComponent(ComponentType::TRANSFORM, &float3(0, 0, 0), &float3(1, 1, 1), &float3(0, 0, 0)));
			objectSelected->components[objectSelected->components.size() - 1]->owner = objectSelected;
		}

		if (ImGui::Button("Set Default"))
		{
			res->metaData.joinVertex = joinVertex = false;
			res->metaData.triangulate = triangulate = true;
			res->metaData.generateNormals = generateNormals = false;
			res->metaData.generateSmoothNormals = generateSmoothNormals = false;
			res->metaData.removeMaterials = removeMaterials = false;
			res->metaData.infacingNormals = infacingNormals = false;
			res->metaData.genUvCoords = generateUvsCoords = true;
			res->metaData.transUvCoords = transformUvsCoords = false;
			res->metaData.findInstances = findInstances = true;
			res->metaData.optimizeMesh = optimizeMesh = true;
			res->metaData.flipUvs = flipUvs = true;
		}

		ImGui::End();
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

		ImGui::Text("Chaos Engine v0.8");
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
		ImGui::Text("- SDL v%d.%d.%d", compiled.major, compiled.minor, compiled.patch);
		ImGui::Text("- Glew v7.0");
		ImGui::Text("- ImGui v1.84.2");
		ImGui::Text("- MathGeoLib v1.5");
		ImGui::Text("- DevIL v1.8.0");
		ImGui::Text("- OpenGL v%s", glGetString(GL_VERSION));
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

	//////////////////////////////////////////////////////////////////////////////////////////// VIEWPORT2 WINDOW ////////////////////////////////////////////////////////////////////////////////////////////

	if (showScene2Window && App->camera->GameCam != nullptr)
	{
		ImGui::CloseCurrentPopup();
		ImGui::Begin("Game Camera", &showScene2Window, ImGuiWindowFlags_NoScrollbar);

		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		if (viewportSize.x != App->camera->GameCam->size.x || viewportSize.y != App->camera->GameCam->size.y)
		{
			App->viewportBuffer->Resize(viewportSize.x, viewportSize.y, App->camera->GameCam);
			App->camera->GameCam->size = { viewportSize.x, viewportSize.y };
			App->renderer3D->OnResize(viewportSize.x, viewportSize.y);
			//App->camera->aspectRatio = viewportSize.x / viewportSize.y;
		}
		viewport = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight() };
		ImGui::Image((ImTextureID)App->camera->GameCam->texture, { App->camera->GameCam->size.x, App->camera->GameCam->size.y }, ImVec2(0, 1), ImVec2(1, 0));

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const char* path = (const char*)payload->Data;
				u32 importedFile = App->resources->ImportFile(path);
				Resource* newResource = App->resources->GetResource(importedFile);

				if (newResource->type == ResourceType::MESH)
				{
					static uint importedGobjs = 1;
					App->scene->gameObjects.push_back(App->scene->CreateGameObject(false, importedGobjs, "Game Object"));
					importedGobjs++;
					objectSelected = App->scene->gameObjects[App->scene->gameObjects.size() - 1];
					App->resources->LoadResource(importedFile,objectSelected);
					objectSelected->components[objectSelected->components.size() - 1]->owner = objectSelected;
					objectSelected->components.push_back(objectSelected->CreateComponent(ComponentType::TRANSFORM, &float3(0, 0, 0), &float3(1, 1, 1), &float3(0, 0, 0)));
					objectSelected->components[objectSelected->components.size() - 1]->owner = objectSelected;
					u32 tex = App->resources->ImportFile("Assets/Textures/Checker.png");
					App->resources->LoadResource(tex,objectSelected);
					objectSelected->components[objectSelected->components.size() - 1]->owner = objectSelected;
					objectSelected->components.erase(objectSelected->components.begin() + 2);
				}
				else if (newResource->type == ResourceType::TEXTURE)
				{
					if (App->editor->objectSelected != nullptr)
					{
						int oldMaterialId;
						oldMaterialId = App->editor->objectSelected->SearchComponent(App->editor->objectSelected, ComponentType::MATERIAL);
						if (oldMaterialId != -1)
						{
							objectSelected->components.erase(objectSelected->components.begin() + oldMaterialId);
						}
						App->resources->LoadResource(importedFile,objectSelected);
						objectSelected->components[objectSelected->components.size() - 1]->owner = objectSelected;
					}
				}
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::End();
	}

	//////////////////////////////////////////////////////////////////////////////////////////// VIEWPORT WINDOW ////////////////////////////////////////////////////////////////////////////////////////////

	if (showSceneWindow)
	{
		ImGui::CloseCurrentPopup();
		ImGui::Begin("Scene", &showSceneWindow, ImGuiWindowFlags_NoScrollbar);

		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		if (viewportSize.x != App->camera->editorCam->size.x || viewportSize.y != App->camera->editorCam->size.y)
		{
			App->viewportBuffer->Resize(viewportSize.x, viewportSize.y, App->camera->editorCam);
			App->camera->editorCam->size = { viewportSize.x, viewportSize.y };
			App->renderer3D->OnResize(viewportSize.x, viewportSize.y);
			//App->camera->aspectRatio = viewportSize.x / viewportSize.y;
		}
		viewport = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight() };
		ImGui::Image((ImTextureID)App->camera->editorCam->texture, { App->camera->editorCam->size.x, App->camera->editorCam->size.y }, ImVec2(0, 1), ImVec2(1, 0));

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const char* path = (const char*)payload->Data;
				newResource = App->resources->GetResource(App->resources->Find(path));

				if (newResource == nullptr)
				{
					std::string isScene = std::string(path);
					unsigned lastPoint = isScene.find_last_of(".");
					isScene = isScene.substr(lastPoint, isScene.length() - lastPoint);
					if (isScene == ".chaos") LoadScene(path);
				}
				else
				{
					if (newResource->type == ResourceType::MESH)
					{
						showMeshMenu = true;
					}
					else if (newResource->type == ResourceType::TEXTURE)
					{
						if (App->editor->objectSelected != nullptr)
						{
							int oldMaterialId;
							oldMaterialId = App->editor->objectSelected->SearchComponent(App->editor->objectSelected, ComponentType::MATERIAL);
							if (oldMaterialId != -1)
							{
								objectSelected->components.erase(objectSelected->components.begin() + oldMaterialId);
							}
							App->resources->LoadResource(newResource->GetUID(),objectSelected);
							objectSelected->components[objectSelected->components.size() - 1]->owner = objectSelected;
							droppedTexture = (ImTextureID)newResource->GetTextureId();
							showTextureMenu = true;
						}
					}
				}
			}
			ImGui::EndDragDropTarget();
		}

		//Guizmos
		if (objectSelected && guizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			ComponentCamera* myCamera = App->camera->editorCam;
			float4x4 cameraProjection = myCamera->frustum.ProjectionMatrix();
			float4x4 cameraView = myCamera->frustum.ViewMatrix();

			if (objectSelected->SearchComponent(objectSelected,ComponentType::TRANSFORM) != -1)
			{
				ComponentTransform* transComponent = objectSelected->getTransform();
				float4x4 transformMatrix = transComponent->transMatrix;
				float3 originalRotation = transComponent->rotationEuler;

				switch (guizmoType)
				{
				case ImGuizmo::OPERATION::TRANSLATE:
					ImGuizmo::Manipulate(cameraView.Transposed().ptr(), cameraProjection.Transposed().ptr(), (ImGuizmo::OPERATION)guizmoType, ImGuizmo::WORLD, transformMatrix.ptr());
					break;
				case ImGuizmo::OPERATION::ROTATE:
					ImGuizmo::Manipulate(cameraView.Transposed().ptr(), cameraProjection.Transposed().ptr(), (ImGuizmo::OPERATION)guizmoType, ImGuizmo::LOCAL, transformMatrix.ptr());
					break;
				case ImGuizmo::OPERATION::SCALE:
					ImGuizmo::Manipulate(cameraView.Transposed().ptr(), cameraProjection.Transposed().ptr(), (ImGuizmo::OPERATION)guizmoType, ImGuizmo::LOCAL, transformMatrix.ptr());
					break;
				default:
					break;
				}

				bool mouseReleased = false;
				if (ImGuizmo::IsUsing())
				{
					transComponent->transMatrix = transformMatrix;
					float3 pos, scale;
					Quat rot;
					transComponent->transMatrix.Transposed().Decompose(pos, rot, scale);
					transComponent->position = pos;
					transComponent->rotationEuler = transComponent->FromQuatToEuler(rot);
					transComponent->scale = scale;
					float3 rotEuler = transComponent->FromQuatToEuler(rot);
				}
				if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
					transComponent->Update(true);
				else
					transComponent->Update(false);
			}
			else
			{
				ComponentTransform2D* transComponent2D = objectSelected->getTransform2D();
				float4x4 transformMatrix = transComponent2D->transMatrix;
				float3 originalRotation = transComponent2D->rotationEuler;

				switch (guizmoType)
				{
				case ImGuizmo::OPERATION::TRANSLATE:
					ImGuizmo::Manipulate(cameraView.Transposed().ptr(), cameraProjection.Transposed().ptr(), (ImGuizmo::OPERATION)guizmoType, ImGuizmo::WORLD, transformMatrix.ptr());
					break;
				case ImGuizmo::OPERATION::ROTATE:
					ImGuizmo::Manipulate(cameraView.Transposed().ptr(), cameraProjection.Transposed().ptr(), (ImGuizmo::OPERATION)guizmoType, ImGuizmo::LOCAL, transformMatrix.ptr());
					break;
				case ImGuizmo::OPERATION::SCALE:
					ImGuizmo::Manipulate(cameraView.Transposed().ptr(), cameraProjection.Transposed().ptr(), (ImGuizmo::OPERATION)guizmoType, ImGuizmo::LOCAL, transformMatrix.ptr());
					break;
				default:
					break;
				}

				bool mouseReleased = false;
				if (ImGuizmo::IsUsing())
				{
					transComponent2D->transMatrix = transformMatrix;
					float3 pos, scale;
					Quat rot;
					transComponent2D->transMatrix.Transposed().Decompose(pos, rot, scale);
					transComponent2D->position = pos;
					transComponent2D->rotationEuler = transComponent2D->FromQuatToEuler(rot);
					transComponent2D->scale = scale;
					float3 rotEuler = transComponent2D->FromQuatToEuler(rot);
				}
				if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
					transComponent2D->Update(true);
				else
					transComponent2D->Update(false);
			}
		}
		ImGui::End();
	}

	//////////////////////////////////////////////////////////////////////////////////////////// ASSETS WINDOW ////////////////////////////////////////////////////////////////////////////////////////////
	const  std::string libraryPath = "Assets/";
	static std::string prevPath = "";
	static std::string currentPath = libraryPath;
	std::vector<std::string> fileList;
	std::vector<std::string> dirList;

	if (showAssets)
	{
		ImGui::CloseCurrentPopup();
		ImGui::Begin("Content Browser", &showAssets);

		static float padding = 1.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;
		ImGui::Columns(columnCount, 0, false);

		//Fill my list of files and folders
		App->fileSystem->DiscoverFiles(currentPath.c_str(), fileList, dirList);
		if (libraryPath != currentPath)
		{
			if (ImGui::ImageButton(backIcon, ImVec2(30, 30), ImVec2(0, 0), ImVec2(1, 1), -1))
			{
				prevPath = currentPath;
				currentPath = libraryPath;
			}
			ImGui::NextColumn();
		}
		for (int i = 0; i < dirList.size(); i++)
		{
			ImGui::PushID(dirList[i].c_str());
			if (ImGui::ImageButton(folderIcon, ImVec2(50, 50), ImVec2(0, 0), ImVec2(1, 1), -1))
			{
				prevPath = currentPath;
				std::string newDirectory = currentPath + dirList[i] + '/';
				currentPath = newDirectory;
				toDelete = currentPath;
			}
			ImGui::Text(dirList[i].c_str());
			ImGui::PopID();
			ImGui::NextColumn();
		}
		for (int i = 0; i < fileList.size(); i++)
		{
			ImGui::PushID(fileList[i].c_str());
			if (ImGui::ImageButton(fileIcon, ImVec2(50, 50), ImVec2(0, 0), ImVec2(1, 1), -1))
			{
				toDelete = currentPath + fileList[i];
			}
			if (ImGui::BeginDragDropSource())
			{
				std::string itemPath = currentPath + fileList[i];
				size_t size = strlen(itemPath.c_str());
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath.c_str(), size + 1, ImGuiCond_Once);
				ImGui::EndDragDropSource();
			}
			ImGui::Text(fileList[i].c_str());
			ImGui::PopID();
			ImGui::NextColumn();
		}
		ImGui::Columns(1);

		if (App->input->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN && toDelete != "empty")
		{
			u32 toDeleteId = App->resources->Find(toDelete.c_str());
			Resource* toDeleteResource = App->resources->GetResource(toDeleteId);
			if (toDeleteResource != nullptr)
			{
				App->resources->ReleaseResource(toDeleteId);
			}
			else if (toDeleteResource == nullptr)
			{
				std::string isFolder;
				unsigned start = 0;
				start = toDelete.find_last_of(".");
				if (start > 300 || start < 0)
				{
					fileList.clear();
					dirList.clear();
					App->fileSystem->DiscoverFiles(currentPath.c_str(), fileList, dirList);
					for (uint i = 0; i < fileList.size(); i++)
					{
						std::string fileToDelete = currentPath + fileList[i];
						App->fileSystem->Remove(fileToDelete.c_str());
					}
					App->fileSystem->Remove(currentPath.c_str());
					currentPath = libraryPath;
					prevPath = currentPath;
				}
				else
				{
					bool hasRemovedFromAssets = App->fileSystem->Remove(toDelete.c_str());
				}
			}
			toDelete = "empty";
		}
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
			FileDialog fileDialog;
			std::string fileToLoad = fileDialog.LoadScene("Chaos Scene (*.chaos)\0*.chaos\0");
			LoadScene(fileToLoad.c_str());
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

	//////////////////////////////////////////////////////////////////////////////////////////// TIME WINDOW ////////////////////////////////////////////////////////////////////////////////////////////

	if (showTimeWindow)
	{
		ImGui::CloseCurrentPopup();
		ImGui::Begin("Time", &showTimeWindow, ImGuiWindowFlags_NoScrollbar);

		if (App->gameMode == true)
		{
			ImGui::Text("Game Time: %.3f", App->gameTimeNum);
			ImGui::SameLine();
			if (ImGui::ImageButton(stopIcon, ImVec2(40, 40), ImVec2(0, 0), ImVec2(1, 1), -1)) {
				App->stopGameTime = !App->stopGameTime;
				App->editor->AddLog("Game Clock Stops (Stopped at %f)\n", App->gameTimeNum);
				App->gameMode = false;
			}
		}
		ImGui::SameLine();
		if (ImGui::ImageButton(playIcon, ImVec2(40, 40), ImVec2(0, 0), ImVec2(1, 1), -1)) {
			App->playGameTime = !App->playGameTime;
			App->editor->AddLog("Game Clock Starts (Started at %f)\n", App->gameTimeNum);
			App->gameMode = true;

		}
		if (App->gameMode == true)
		{
			ImGui::SameLine();
			if (ImGui::ImageButton(pauseIcon, ImVec2(40, 40), ImVec2(0, 0), ImVec2(1, 1), -1)) {
				App->pauseGameTime = !App->pauseGameTime;
				App->editor->AddLog("Game Clock Paused\n");

			}
			ImGui::SameLine();
			if (ImGui::ImageButton(advanceIcon, ImVec2(40, 40), ImVec2(0, 0), ImVec2(1, 1), -1)) {
				App->advanceGameTime = !App->advanceGameTime;
				App->editor->AddLog("Game Clock Advanced 1 frame\n");

			}
			ImGui::SameLine();
			if (ImGui::ImageButton(speedDownIcon, ImVec2(40, 40), ImVec2(0, 0), ImVec2(1, 1), -1)) {
				App->slowerGameTime = !App->slowerGameTime;
				App->editor->AddLog("Game Clock Speed Down (x%d)\n");

			}
			ImGui::SameLine();
			if (ImGui::ImageButton(speedUpIcon, ImVec2(40, 40), ImVec2(0, 0), ImVec2(1, 1), -1)) {
				App->fasterGameTime = !App->fasterGameTime;
				App->editor->AddLog("Game Clock Speed Up (x%d)\n");

			}
			ImGui::SameLine();
		}

		ImGui::End();
	}

	if (showSaveOnExitMenu)
	{
		ImGui::Begin("Save Scene", &showTimeWindow, ImGuiWindowFlags_NoScrollbar);
		ImGui::Text("Do you want to save the current scene?");
		if (ImGui::Button("Yes, save the scene"))
		{
			FileDialog fileDialog;
			std::string path = fileDialog.SaveScene("Chaos Scene (*.chaos)\0*.chaos\0");
			App->editor->SaveScene(path.c_str());
			return UPDATE_STOP;
		}
		else if (ImGui::Button("No, don't save the scene"))
			return UPDATE_STOP;
		if (ImGui::Button("Cancel"))
			showSaveOnExitMenu = !showSaveOnExitMenu;

		ImGui::End();
	}

	return UPDATE_CONTINUE;
}
update_status  ModuleEditor::PreUpdate(float dt)
{
	if (App->camera->GameCam != nullptr)
	{
		App->camera->GameCam->pre();
	}

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);

	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	App->camera->editorCam->pre();
	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate(float dt)
{

	if (App->camera->GameCam != nullptr) {
		App->camera->GameCam->post();
	}

	App->camera->editorCam->post();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Rendering
	ImGui::Render();


	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
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