#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"

#include "shellapi.h"

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

	showcase = true;
	
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
		//char title[25];
		//sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
		//ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
		//sprintf_s(title, 25, "Millisenconds %0.1f", ms_log[ms_log.size() - 1]);
		//ImGui::PlotHistogram("##milliseconds", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
	}

	if (ImGui::CollapsingHeader("Window"))
	{

	}

	if (ImGui::CollapsingHeader("File System"))
	{

	}

	if (ImGui::CollapsingHeader("Input"))
	{

	}

	if (ImGui::CollapsingHeader("Hardware"))
	{

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