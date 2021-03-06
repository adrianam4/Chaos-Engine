#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"
#include <stdio.h>
#include <SDL.h>

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool startEnabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();

	void SetTitle(const char* title);
	void SetFullscreen(bool fullscreen);
	void SetResizable(bool resizable);
	void SetDekstop(bool dekstop);
	void SetBorderless(bool borderless);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screenSurface;

	int width;
	int height;

private:
	Uint32 flags;
};

#endif // __ModuleWindow_H__