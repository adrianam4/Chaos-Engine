#include "Application.h"
#include "Globals.h"
#include "ModuleEditor.h"

#include "shellapi.h"
#include <GL/GL.h>
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "Parson/parson.h"

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	consoleBuffer.clear();
}

ModuleEditor::~ModuleEditor()
{
	consoleBuffer.clear();
}

// Load assets
bool ModuleEditor::Start()
{
	App->editor->AddLog("Loading Editor Assets\n");
	bool ret = true;

	plane = new MyPlane(0, 0, 0, 0);
	plane->axis = true;
	cube = new MyCube(-5, 0, 0, 1, 1, 1);
	pyramid = new MyPyramid(5, 0, 0, 1, 1, 1);
	cylinder = new MyCylinder();
	sphere = new MySphere(3,20,20);

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

	//delete plane;
	delete cube;
	delete pyramid;
	//delete cylinder;

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
	JSON_Value* user_data = json_parse_file("ConfigFile.json");

	maxFPS = json_object_get_number(json_object(user_data), "MaxFPS");
	App->maxMs = json_object_get_number(json_object(user_data), "MaxMs");
	width = json_object_get_number(json_object(user_data), "Width");
	height = json_object_get_number(json_object(user_data), "Height");
	brightness = json_object_get_number(json_object(user_data), "Brightness");
	fullscreen = json_object_get_boolean(json_object(user_data), "Fullscreen");
	resizable = json_object_get_boolean(json_object(user_data), "Resizable");
	borderless = json_object_get_boolean(json_object(user_data), "Borderless");
	dekstop = json_object_get_boolean(json_object(user_data), "Dekstop");
	wireframe = json_object_get_boolean(json_object(user_data), "Wireframe");
	normals = json_object_get_boolean(json_object(user_data), "Normals");

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

// Update: draw background
update_status ModuleEditor::Update(float dt)
{
	// Our state
	static bool show_demo_window = false;
	static bool show_another_window = false;
	static bool show_close_window = true;
	static bool show_about_window = false;
	static bool open_config_menu = false;
	static bool show_config_menu = true;
	static bool show_console_menu = true;
	static bool is_active = false;
	static bool is_active2 = false;
	ImVec4 clear_color = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		SaveConfig();

	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		LoadConfig();
		ComproveScreen();
	}

	////////////////////////////////////////////////////////////////// MAIN MENU BAR //////////////////////////////////////////////////////////////////

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File", &open_config_menu))
		{
			if (ImGui::MenuItem("Quit"))
			{
				return UPDATE_STOP;
			}
			ImGui::EndMenu();
		}


		if (ImGui::BeginMenu("View", &open_config_menu))
		{
			if (ImGui::MenuItem("Configuration"))
			{
				show_config_menu = !show_config_menu;
			}
			if (ImGui::MenuItem("Console"))
			{
				show_console_menu = !show_console_menu;
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


		if (ImGui::BeginMenu("Help", &open_config_menu))
		{
			if (ImGui::MenuItem("ImGui Demo"))
			{
				show_demo_window = !show_demo_window;
			}

			if (ImGui::MenuItem("Documentation"))
				ShellExecute(NULL, "open", "https://github.com/adrianam4/Chaos-Engine/wiki", NULL, NULL, SW_SHOWNORMAL);

			if (ImGui::MenuItem("Download Latest"))
				ShellExecute(NULL, "open", "https://github.com/adrianam4/Chaos-Engine/releases", NULL, NULL, SW_SHOWNORMAL);

			if (ImGui::MenuItem("Report a bug"))
				ShellExecute(NULL, "open", "https://github.com/adrianam4/Chaos-Engine/issues", NULL, NULL, SW_SHOWNORMAL);
		
			if (ImGui::MenuItem("About"))
			{
				show_about_window = !show_about_window;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}


	////////////////////////////////////////////////////////////////// CONFIGURATION WINDOW //////////////////////////////////////////////////////////////////

	if (show_config_menu)
	{
		ImGui::Begin("Configuration", &open_config_menu);

		if (ImGui::CollapsingHeader("Application"))
		{
			static ImVector<float> fps_log;
			static ImVector<float> ms_log;
			char title[25];

			fps_log.push_back(ImGui::GetIO().Framerate);
			ms_log.push_back(1000 / (ImGui::GetIO().Framerate));

			ImGui::InputText("App Name", TITLE, 25);
			ImGui::InputText("Organization", ORGANIZATION, 25);
			if (ImGui::SliderInt("Max FPS", &maxFPS, 0, 144))
			{
				if (maxFPS > 0)
					App->maxMs = 1000 / maxFPS;
				else
					App->maxMs = 0;
			}

			sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
			ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
			sprintf_s(title, 25, "Miliseconds %.1f", ms_log[ms_log.size() - 1]);
			ImGui::PlotHistogram("##miliseconds", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));

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
			if (ImGui::Checkbox("Active", &is_active))
			{

			}
			ImGui::Text("Icon: *default*");

			ImGui::SliderFloat("Brigthness", &brightness, 0, 1.0f);

			ImGui::SliderInt("Width", &width, 0, 2560);
			ImGui::SliderInt("Heigh", &height, 0, 1440);

			SDL_SetWindowSize(App->window->window, width, height);
			SDL_SetWindowBrightness(App->window->window, brightness);

			int display_count = 0, display_index = 0, mode_index = 0;
			SDL_DisplayMode mode = { SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0 };

			if ((display_count = SDL_GetNumVideoDisplays()) < 1) {
				SDL_Log("SDL_GetNumVideoDisplays returned: %i", display_count);
			}

			if (SDL_GetDisplayMode(display_index, mode_index, &mode) != 0) {
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

		if (ImGui::CollapsingHeader("File System"))
		{
			if (ImGui::Checkbox("Active", &is_active2))
			{
			}

			ImGui::Text("Base Path: ");
			ImGui::TextColored(ImVec4(255, 255, 0, 255), SDL_GetBasePath());

			ImGui::Text("Read Paths: ");
			ImGui::TextColored(ImVec4(255, 255, 0, 255), ".");

			ImGui::Text("Write Path: ");
			ImGui::TextColored(ImVec4(255, 255, 0, 255), ".");

		}

		if (ImGui::CollapsingHeader("Input"))
		{
			if (ImGui::Checkbox("Active", &is_active))
			{

			}

			// Mouse Info
			ImGui::Text("Mouse Position: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d,%d", App->input->GetMouseX(), App->input->GetMouseY());
			ImGui::Text("Mouse Motion: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d,%d", App->input->GetMouseXMotion(), App->input->GetMouseYMotion());
			ImGui::Text("Mouse Wheel: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d", App->input->GetMouseZ());

			ImGui::Separator();

			//Keyboard Info
			static ImVector<int> keys_id;
			static ImVector<const char*> keys_state;

			for (int i = 0; i < 300; ++i)
			{
				if (App->input->keys[i] == 1)
				{
					if (App->input->keyboard[i] == KEY_IDLE)
					{
						keys_id.push_back(i);
						keys_state.push_back("DOWN");
					}
					else
					{
						keys_id.push_back(i);
						keys_state.push_back("REPEAT");
						break;
					}
				}
				else
				{
					if (App->input->keyboard[i] == KEY_REPEAT || App->input->keyboard[i] == KEY_DOWN)
					{
						keys_id.push_back(i);
						keys_state.push_back("UP");
					}
				}
			}

			for (int i = 0; i < keys_id.Size; i++)
			{
				int j = keys_state.Size - i - 1;
				ImGui::Text("Keybr: %d - %s", keys_id[j], keys_state[j]);
			}

		}

		if (ImGui::CollapsingHeader("Hardware"))
		{
			if (ImGui::Checkbox("Active", &is_active))
			{

			}

			SDL_version compiled;
			SDL_version linked;

			SDL_VERSION(&compiled);
			SDL_GetVersion(&linked);

			int cpu_cores = SDL_GetCPUCount();
			int cpu_cache = SDL_GetCPUCacheLineSize();
			float ram = SDL_GetSystemRAM() / 1000;

			ImGui::Text("SDL Version: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d.%d.%d", compiled.major, compiled.minor, compiled.patch);
			ImGui::Separator();
			ImGui::Text("CPUs: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d (Cache: %d kb)", cpu_cores, cpu_cache);
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

		ImGui::End();
	}
	
	////////////////////////////////////////////////////////////////// CONSOLE WINDOW //////////////////////////////////////////////////////////////////
	
	if (show_console_menu)
	{
		ImGui::Begin("Console");

		ImGui::TextUnformatted(consoleBuffer.begin());

		if (scrollToBottom)
			ImGui::SetScrollHereY(1.0f);
		
		scrollToBottom = false;
		ImGui::End();
	}

	if (show_about_window)
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

	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate(float dt)
{
	plane->Render();

	cube->DrawCube();

	pyramid->DrawPyramid();

	//cylinder->DrawCylinder();

	//sphere->DrawSphere();

	return UPDATE_CONTINUE;
}