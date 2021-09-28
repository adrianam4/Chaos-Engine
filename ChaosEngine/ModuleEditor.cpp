#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"

#include "shellapi.h"

#include <GL/GL.h>
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl2.h"

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleEditor::~ModuleEditor()
{}

// Load assets
bool ModuleEditor::Start()
{
	LOGCE("Loading Editor Assets");
	bool ret = true;
	
	return ret;
}

// Load assets
bool ModuleEditor::CleanUp()
{
	LOGCE("Unloading Editor scene");

	return true;
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
	static bool fullscreen = false;
	static bool resizable = true;
	static bool borderless = false;
	static bool dekstop = false;
	static bool isActive = false;
	ImVec4 clear_color = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

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
				open_config_menu = !open_config_menu;
			}
			if (ImGui::MenuItem("Console"))
			{
				
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

	ImGui::Begin("Configuration", &open_config_menu);

	if (ImGui::CollapsingHeader("Application"))
	{
		static ImVector<float> fps_log;
		static ImVector<float> ms_log;
		static int maxFPS = 144;
		char title[25];

		fps_log.push_back(ImGui::GetIO().Framerate);
		ms_log.push_back(1000 / (ImGui::GetIO().Framerate));

		ImGui::Text("App Name: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%s", TITLE);
		ImGui::Text("Organization: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%s", ORGANIZATION);
		ImGui::SliderInt("Max FPS", &maxFPS, 0, 144);

		sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
		ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
		sprintf_s(title, 25, "Miliseconds %.1f", ms_log[ms_log.size() - 1]);
		ImGui::PlotHistogram("##miliseconds", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
	}

	if (ImGui::CollapsingHeader("Window"))
	{
		if (ImGui::Checkbox("Active", &isActive))
		{

		}
		ImGui::Text("Icon: *default*");
		
		float brightness = SDL_GetWindowBrightness(App->window->window);
		ImGui::SliderFloat("Brigthness", &brightness, 0, 1.0f);
		int width, height;
		SDL_GetWindowSize(App->window->window,&width,&height); 
		ImGui::SliderInt("Width",&width, 0, 2560);
		ImGui::SliderInt("Heigh",&height, 0, 1440);
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

	}

	if (ImGui::CollapsingHeader("Input"))
	{
		if (ImGui::Checkbox("Active", &isActive))
		{

		}

		// Mouse Info
		ImGui::Text("Mouse Position: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d,%d", App->input->GetMouseX(),App->input->GetMouseY());
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
			ImGui::Text("Keybr: %d - %s", keys_id[j],keys_state[j]);
		}

	}

	if (ImGui::CollapsingHeader("Hardware"))
	{
		if (ImGui::Checkbox("Active", &isActive))
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
		
		const GLubyte* renderer = glGetString(GL_RENDER);
		const GLubyte* vendor = glGetString(GL_VENDOR);

		ImGui::Text("GPUs: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%s", vendor);
		ImGui::Text("Brand: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%s", renderer);
		ImGui::Text("VRAM Budget: ");
		ImGui::Text("VRAM Usage: ");
		ImGui::Text("VRAM Available: ");
		ImGui::Text("VRAM Reserved: ");
	}

	ImGui::End();

	//if (!ImGui::Begin("Configuration", &open_config_menu))
	//{
	//	ImGui::End();
	//}

	//if (ImGui::CollapsingHeader("Application"))
	//{

	//}

	//// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	//{
	//	static float f = 0.0f;
	//	static int counter = 0;

	//	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	//	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	//	ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
	//	ImGui::Checkbox("Another Window", &show_another_window);

	//	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	//	ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

	//	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	//		counter++;
	//	ImGui::SameLine();
	//	ImGui::Text("counter = %d", counter);

	//	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	//	ImGui::End();
	//}

	//// 3. Show another simple window.
	//if (show_another_window)
	//{
	//	ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
	//	ImGui::Text("Hello from another window!");
	//	if (ImGui::Button("Close Me"))
	//		show_another_window = false;
	//	ImGui::End();

	if (show_about_window)
		ImGui::ShowAboutWindow(&show_about_window);

	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate(float dt)
{


	return UPDATE_CONTINUE;
}