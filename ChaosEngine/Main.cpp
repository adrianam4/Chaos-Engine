#include <stdlib.h>
#include "Application.h"
#include "Globals.h"

#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )
#include "mmgr.h"

enum main_states
{
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

Application* App = nullptr;

int main(int argc, char ** argv)
{
	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
		case MAIN_CREATION:
			App = new Application();
			App->editor->AddLog("-------------- Application Start --------------\n");
			App->editor->AddLog("Starting game '%s'...\n", TITLE);
			App->editor->AddLog("-------------- Application Creation --------------\n");
			state = MAIN_START;
			break;

		case MAIN_START:

			App->editor->AddLog("-------------- Application Init --------------\n");
			if (App->Init() == false)
			{
				App->editor->AddLog("Application Init exits with ERROR\n");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				App->editor->AddLog("-------------- Application Update --------------\n");
			}

			break;

		case MAIN_UPDATE:
		{
			int update_return = App->Update();

			if (update_return == UPDATE_ERROR)
			{
				App->editor->AddLog("Application Update exits with ERROR\n");
				state = MAIN_EXIT;
			}

			if (update_return == UPDATE_STOP)
				state = MAIN_FINISH;
		}
			break;

		case MAIN_FINISH:
			App->editor->AddLog("-------------- Application CleanUp --------------\n");
			if (App->CleanUp() == false)
			{
				App->editor->AddLog("Application CleanUp exits with ERROR\n");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}
	}

	App->editor->AddLog("Exiting game 'Chaos Engine'...\n");
	delete App;

	return main_return;
}