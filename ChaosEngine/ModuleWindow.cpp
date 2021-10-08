#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{}

// Called before render is available
bool ModuleWindow::Init()
{
	SDL_GL_CreateContext(window);

	App->editor->AddLog("Init SDL window & surface\n");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		App->editor->AddLog("SDL_VIDEO could not initialize! SDL_Error: %s\n");
		ret = false;
	}
	else
	{
		// Setup SDL
        // (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows systems,
        // depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to latest version of SDL is recommended!)
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
		{
			printf("Error: %s\n", SDL_GetError());
			return -1;
		}

		//Create window
		width = SCREEN_WIDTH * SCREEN_SIZE;
		height = SCREEN_HEIGHT * SCREEN_SIZE;
		flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 3.3
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			App->editor->AddLog("Window could not be created! SDL_Error: %s\n");
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

update_status ModuleWindow::Update(float dt) 
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	SDL_GL_DeleteContext(window);

	App->editor->AddLog("Destroying SDL window and quitting all SDL systems\n");
	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::SetFullscreen(bool fullscreen)
{
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
		SDL_SetWindowFullscreen(window, flags);
	}
	else
	{
		flags = SDL_WINDOW_RESIZABLE;
		SDL_SetWindowFullscreen(window, flags);
	}
}

void ModuleWindow::SetResizable(bool resizable)
{
	if (resizable)
	{
		flags = SDL_WINDOW_RESIZABLE;
		SDL_SetWindowFullscreen(window, flags);
	}
}

void ModuleWindow::SetDekstop(bool dekstop)
{
	if (dekstop)
	{
		flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
		SDL_SetWindowFullscreen(window, flags);
	}
	else
	{
		flags = SDL_WINDOW_RESIZABLE;
		SDL_SetWindowFullscreen(window, flags);
	}
}

void ModuleWindow::SetBorderless(bool borderless)
{
	if (borderless)
	{
		flags = SDL_WINDOW_BORDERLESS;
		SDL_SetWindowFullscreen(window, flags);
	}
	else
	{
		flags = SDL_WINDOW_RESIZABLE;
		SDL_SetWindowFullscreen(window, flags);
	}
}
