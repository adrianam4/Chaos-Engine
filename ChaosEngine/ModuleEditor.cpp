#include "Application.h"
#include "Globals.h"
#include "ModuleEditor.h"
#include "ModuleScene.h"

#include "shellapi.h"
#include <GL/GL.h>
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "Parson/parson.h"

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

	plane = new MyPlane(0, 0, 0, 0);
	plane->axis = true;

	App->camera->Move(Vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(Vec3(0, 0, 0));
	
	LoadConfig();
	ComproveScreen();

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
	json_object_set_boolean(json_object(user_data), "Wireframe", wireframe);
	json_object_set_boolean(json_object(user_data), "Normals", normals);

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
	wireframe = json_object_get_boolean(json_object(userData), "Wireframe");
	normals = json_object_get_boolean(json_object(userData), "Normals");

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

void ModuleEditor::AddSphere(float3 pos, float3 sca, float radius, uint rings, uint sectors)
{
	MySphere* auxSphere;
	auxSphere = new MySphere(pos, sca, 2, 20, 20);
	spheres.push_back(auxSphere);
}

void ModuleEditor::AddCylinder(float3 pos, float3 sca)
{
	MyCylinder* auxCylinder;
	auxCylinder = new MyCylinder(pos,sca);
	cylinders.push_back(auxCylinder);
}

// Update: draw background
update_status ModuleEditor::Update(float dt)
{
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
	static bool isActive = true;
	static bool isActive2 = true;
	static bool isActive3 = true;
	static bool isActive4 = true;
	ImVec4 clearColor = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

	if (App->input->GetKey(SDL_SCANCODE_F10))
		SaveConfig();

	if (App->input->GetKey(SDL_SCANCODE_F11))
	{
		LoadConfig();
		ComproveScreen();
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
			if (ImGui::MenuItem("Enable/Disable Wireframe"))
			{
				wireframe = !wireframe;
			}
			if (ImGui::MenuItem("Enable/Disable Normals"))
			{
				normals = !normals;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("GameObject", &openConfigMenu))
		{
			if (ImGui::MenuItem("Create Empty"))
			{
				App->scene->gameObjects.push_back(App->scene->CreateGameObject("Empty GameObject", false));
				int lastComponent = App->scene->gameObjects.size() - 1;
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::TRANSFORM));
				App->scene->gameObjects[lastComponent]->components[0]->owner = App->scene->gameObjects[lastComponent];
				
			}
			if (ImGui::MenuItem("Create Mesh"))
			{
				App->scene->gameObjects.push_back(App->scene->CreateGameObject("Mesh", false));
				int lastComponent = App->scene->gameObjects.size() - 1;
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::MESH));
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::TRANSFORM));
				App->scene->gameObjects[lastComponent]->components[0]->owner = App->scene->gameObjects[lastComponent];
				App->scene->gameObjects[lastComponent]->components[1]->owner = App->scene->gameObjects[lastComponent];
			}
			if (ImGui::MenuItem("Create Cube"))
			{
				App->scene->gameObjects.push_back(App->scene->CreateGameObject("Cube", false));
				int lastComponent = App->scene->gameObjects.size() - 1;
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::CUBE));
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::TRANSFORM));
				App->scene->gameObjects[lastComponent]->components[0]->owner = App->scene->gameObjects[lastComponent];
				App->scene->gameObjects[lastComponent]->components[1]->owner = App->scene->gameObjects[lastComponent];
			}
			if (ImGui::MenuItem("Create Pyramid"))
			{
				App->scene->gameObjects.push_back(App->scene->CreateGameObject("Pyramid", false));
				int lastComponent = App->scene->gameObjects.size() - 1;
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::PYRAMID));
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::TRANSFORM));
				App->scene->gameObjects[lastComponent]->components[0]->owner = App->scene->gameObjects[lastComponent];
				App->scene->gameObjects[lastComponent]->components[1]->owner = App->scene->gameObjects[lastComponent];
			}
			if (ImGui::MenuItem("Create Sphere"))
			{
				App->scene->gameObjects.push_back(App->scene->CreateGameObject("Sphere", false));
				int lastComponent = App->scene->gameObjects.size() - 1;
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::SPHERE));
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::TRANSFORM));
				App->scene->gameObjects[lastComponent]->components[0]->owner = App->scene->gameObjects[lastComponent];
				App->scene->gameObjects[lastComponent]->components[1]->owner = App->scene->gameObjects[lastComponent];
			}
			if (ImGui::MenuItem("Create Cylinder"))
			{
				App->scene->gameObjects.push_back(App->scene->CreateGameObject("Cylinder", false));
				int lastComponent = App->scene->gameObjects.size() - 1;
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::CYLINDER));
				App->scene->gameObjects[lastComponent]->components.push_back(App->scene->gameObjects[lastComponent]->CreateComponent(ComponentType::TRANSFORM));
				App->scene->gameObjects[lastComponent]->components[0]->owner = App->scene->gameObjects[lastComponent];
				App->scene->gameObjects[lastComponent]->components[1]->owner = App->scene->gameObjects[lastComponent];
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
		ImGui::EndMainMenuBar();
	}

	//////////////////////////////////////////////////////////////////////////////////////////// HIERARCHY WINDOW ////////////////////////////////////////////////////////////////////////////////////////////

	if (showHierarchy)
	{
		ImGui::Begin("Hierarchy", &openConfigMenu); // ------ BEGIN HIERARCHY
		ImGui::LabelText("", "Game Objects in Scene: %d", App->scene->gameObjects.size());
		
		for (int i = 0; i < App->scene->gameObjects.size(); i++)
		{
			if (ImGui::TreeNode(App->scene->gameObjects[i]->name))
			{
				if (ImGui::IsItemHovered() && ImGui::GetIO().MouseClicked[0])
				{
					objectSelected = App->scene->gameObjects[i];

				}

				for (int j = 0; j < App->scene->gameObjects[i]->childrens.size(); j++)
				{
					if (App->scene->gameObjects[i]->childrens.size() > 0)
					{
						if (ImGui::TreeNode(App->scene->gameObjects[i]->childrens[j]->name))
						{
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
					objectSelected = App->scene->gameObjects[i];

				}
			}
			ImGui::Separator();
		}
		
		ImGui::End(); // ------ END HIERARCHY
	}

	if (showInspector && objectSelected != nullptr)
	{
		ImGui::Begin("Inspector", &openConfigMenu);

		ImGui::TextColored(ImVec4(255, 255, 0, 255), objectSelected->name);
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
		ImGui::Begin("Configuration", &openConfigMenu);

		if (ImGui::CollapsingHeader("Application"))
		{
			static ImVector<float> fpsLog;
			static ImVector<float> msLog;
			char title[25];

			fpsLog.push_back(ImGui::GetIO().Framerate);
			msLog.push_back(1000 / (ImGui::GetIO().Framerate));

			ImGui::InputText("App Name", TITLE, 25);
			ImGui::InputText("Organization", ORGANIZATION, 25);
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

			ImGui::Text("Total Reported Mem: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d");
			ImGui::Text("Total Actual Mem: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d");
			ImGui::Text("Peak Reported Mem: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d");
			ImGui::Text("Peak Actual Mem: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d");
			ImGui::Text("Accumulated Reported Mem: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d");
			ImGui::Text("Accumulated Actual Mem: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d");
			ImGui::Text("Accumulated Alloc Unit Count: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d");
			ImGui::Text("Total Alloc Unit Count: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d");
			ImGui::Text("Peak Alloc Unit Count: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d");
		}

		if (ImGui::CollapsingHeader("Window"))
		{
			if (ImGui::Checkbox("Active", &isActive))
			{

			}
			if (isActive)
			{

				ImGui::Text("Icon: *default*");

				ImGui::SliderFloat("Brigthness", &brightness, 0, 1.0f);

				ImGui::SliderInt("Width", &width, 0, 2560);
				ImGui::SliderInt("Heigh", &height, 0, 1440);

				SDL_SetWindowSize(App->window->window, width, height);
				SDL_SetWindowBrightness(App->window->window, brightness);

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
				ImGui::TextColored(ImVec4(255, 255, 0, 255), ".");

				ImGui::Text("Write Path: ");
				ImGui::TextColored(ImVec4(255, 255, 0, 255), ".");
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

				ImGui::Text("GPUs: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%s", renderer);
				ImGui::Text("Brand: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%s", vendor);
				ImGui::Text("VRAM Budget: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d Mb");
				ImGui::Text("VRAM Usage: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d Mb");
				ImGui::Text("VRAM Available: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d Mb");
				ImGui::Text("VRAM Reserved: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d Mb");
			}
		}

		ImGui::End();
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////// CONSOLE WINDOW ////////////////////////////////////////////////////////////////////////////////////////////
	
	if (showConsoleMenu)
	{
		ImGui::Begin("Console");

		ImGui::TextUnformatted(consoleBuffer.begin());

		if (scrollToBottom)
			ImGui::SetScrollHereY(1.0f);
		
		scrollToBottom = false;
		ImGui::End();
	}

	if (showAboutWindow)
	{
		ImGui::Begin("About");

		ImGui::Text("Chaos Engine v0.1");
		ImGui::Separator();
		ImGui::Text("A trully amazing 3D Game Engine.");
		ImGui::Text("Created by David Lira and Adrian Aroca for the Game Engines subject.");
		ImGui::Text("David Lira Github: https://github.com/davidlira19");
		ImGui::Text("Adrian Aroca Github: https://github.com/adrianam4");
		ImGui::Separator();
		ImGui::Text("3rd Party Libraries used:");
		ImGui::Text("");
		SDL_version compiled; SDL_version linked; SDL_VERSION(&compiled); SDL_GetVersion(&linked);
		ImGui::Text("- SDL v%d.%d.%d",compiled.major, compiled.minor, compiled.patch);
		ImGui::Text("- Glew v7.0");
		ImGui::Text("- ImGui v1.84.2");
		ImGui::Text("- MathGeoLib v1.5");
		ImGui::Text("- OpenGL v%s",glGetString(GL_VERSION));
		ImGui::Text("");
		ImGui::Separator();
		ImGui::Text("License:");
		ImGui::Text("");
		ImGui::Text("MIT License");
		ImGui::Text("");
		ImGui::Text("Copyright (c) 2021 adrianam4 and davidlira19");
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
	plane->DrawPlane();

	for (int i = 0; i < App->scene->gameObjects.size(); i++)
	{
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
		}
	}

	return UPDATE_CONTINUE;
}