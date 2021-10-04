#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "GL/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <vector>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	App->editor->AddLog("Creating 3D Renderer context\n");
	bool ret = true;
	//Create context
	context = SDL_GL_CreateContext(App->window->window);

	if(context == NULL)
	{
		App->editor->AddLog("OpenGL context could not be created! SDL_Error: %s\n");
		ret = false;
	}
	

	if(ret == true)
	{
		GLenum err = glewInit();
		// … check for errors
		App->editor->AddLog("Using Glew %s\n", glewGetString(GLEW_VERSION));
		// Should be 2.0
		App->editor->AddLog("Vendor: %s\n", glGetString(GL_VENDOR));
		App->editor->AddLog("Renderer: %s\n", glGetString(GL_RENDERER));
		App->editor->AddLog("OpenGL version supported %s\n", glGetString(GL_VERSION));
		App->editor->AddLog("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

		if (GLEW_OK != err)
		{
			/* Problem: glewInit failed, something is seriously wrong. */
			App->editor->AddLog("Error: %s\n", glewGetErrorString(err));
		}
		App->editor->AddLog("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

		//Use Vsync
		if (VSYNC && SDL_GL_SetSwapInterval(1) < 0)
		{
			App->editor->AddLog("Warning: Unable to set VSync! SDL Error: %s\n");
		}

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Setup Platform/Renderer backends
		ImGui_ImplSDL2_InitForOpenGL(App->window->window, context);
		ImGui_ImplOpenGL3_Init();
		
		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			App->editor->AddLog("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			App->editor->AddLog("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			App->editor->AddLog("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

		lights[0].Active(true);

	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	static bool depth;
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		depth = !depth;
		if (depth)
		{
			glDisable(GL_DEPTH_TEST);
			App->editor->AddLog("GL_DEPTH_TEST disabled \n");
		}
		else
		{
			glEnable(GL_DEPTH_TEST);
			App->editor->AddLog("GL_DEPTH_TEST enabled \n");
		}
	}

	static bool cull;
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		cull = !cull;
		if (cull)
		{
			glDisable(GL_CULL_FACE);
			App->editor->AddLog("GL_CULL_FACE disabled \n");
		}
		else
		{
			glEnable(GL_CULL_FACE);
			App->editor->AddLog("GL_CULL_FACE enabled \n");
		}
	}

	static bool lighting;
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		lighting = !lighting;
		if (lighting)
		{
			glDisable(GL_LIGHTING);
			App->editor->AddLog("GL_LIGHTING disabled \n");
		}
		else
		{
			glEnable(GL_LIGHTING);
			App->editor->AddLog("GL_LIGHTING enabled \n");
		}

	}

	static bool color_material;
	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
	{
		color_material = !color_material;
		if (color_material)
		{
			glDisable(GL_COLOR_MATERIAL);
			App->editor->AddLog("GL_COLOR_MATERIAL disabled \n");
		}
		else
		{
			glEnable(GL_COLOR_MATERIAL);
			App->editor->AddLog("GL_COLOR_MATERIAL enabled \n");
		}
	}

	static bool texture;
	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		texture = !texture;
		if (texture)
		{
			glDisable(GL_TEXTURE_2D);
			App->editor->AddLog("GL_TEXTURE_2D disabled \n");
		}
		else
		{
			glEnable(GL_TEXTURE_2D);
			App->editor->AddLog("GL_TEXTURE_2D enabled \n");
		}
	}


	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	// Rendering
	ImGui::Render();
	/*glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);*/
	//glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound
	

	
	//----------Direct Mode--------------------------------------------------------------------------------------------------------------
	
	/*const GLfloat v0[] = {-1, -1, 0};
	const GLfloat v1[] = { 1, -1, 0 };
	const GLfloat v2[] = { 1, 1, 0 };
	const GLfloat v3[] = { -1, 1, 0 };
	
	const GLfloat v4[] = { -1, -1, -2 };
	const GLfloat v5[] = { 1, -1, -2 };
	const GLfloat v6[] = { 1, 1, -2 };
	const GLfloat v7[] = { -1, 1, -2 };
	glBegin(GL_TRIANGLES);
	//FRONT
	glVertex3fv(v0);
	glVertex3fv(v1);
	glVertex3fv(v2);

	glVertex3fv(v0);
	glVertex3fv(v2);
	glVertex3fv(v3);

	//BACK
	glVertex3fv(v6);
	glVertex3fv(v5);
	glVertex3fv(v4);

	glVertex3fv(v7);
	glVertex3fv(v6);
	glVertex3fv(v4);

	//RIGHT
	glVertex3fv(v1);
	glVertex3fv(v5);
	glVertex3fv(v6);

	glVertex3fv(v1);
	glVertex3fv(v6);
	glVertex3fv(v2);

	//LEFT
	glVertex3fv(v4);
	glVertex3fv(v0);
	glVertex3fv(v3);

	glVertex3fv(v4);
	glVertex3fv(v3);
	glVertex3fv(v7);

	//TOP
	glVertex3fv(v3);
	glVertex3fv(v2);
	glVertex3fv(v6);

	glVertex3fv(v3);
	glVertex3fv(v6);
	glVertex3fv(v7);

	//DOWN
	glVertex3fv(v5);
	glVertex3fv(v1);
	glVertex3fv(v0);

	glVertex3fv(v4);
	glVertex3fv(v5);
	glVertex3fv(v0);

	glEnd();*/

	//----------Array Mode--------------------------------------------------------------------------------------------------------------
	/*static float vertices[] =
	{ -1, -1, 0,
	  1, -1, 0,
	  1, 1, 0,
	  -1, -1, 0,
	  1, 1, 0 ,
	  -1, 1, 0 ,
	  1, 1, -2 ,
	  1, -1, -2 ,
	   -1, -1, -2,
	   -1, 1, -2,
	   1, 1, -2,
		-1, -1, -2,
		1, -1, 0,
		1, -1, -2,
		1, 1, -2,
		1, -1, 0,
		1, 1, -2,
		1, 1, 0,
		-1, -1, -2,
		-1, -1, 0,
		-1, 1, 0,
		-1, -1, -2,
		-1, 1, 0,
		-1, 1, -2,
		 -1, 1, 0,
		1, 1, 0,
		 1, 1, -2,
		-1, 1, 0,
		1, 1, -2,
		-1, 1, -2,
		1, -1, -2,
		1, -1, 0,
		-1, -1, 0,
		-1, -1, -2,
		1, -1, -2,
		-1, -1, 0
	};

	static int num_vertices = 36;
	static uint my_id = 0;

	glGenBuffers(1, (GLuint*)&(my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices * 3, vertices, GL_STATIC_DRAW);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_TRIANGLES, 0, num_vertices);
	glDisableClientState(GL_VERTEX_ARRAY);*/
	
	//-------------------------------------------------- Indices mode ----------------------------------------------------------------------------------------------------
	static GLfloat vertices[] =
	{
		-1, -1, 0,
		1, -1, 0,
		1, 1, 0,
		-1, 1, 0,

		-1, -1, -2,
		1, -1, -2,
		1, 1, -2,
		-1, 1, -2
	};

	static GLuint indices[] =
	{
		0,1,2, 0,2,3,
		6,5,4, 7,6,4,
		1,5,6, 1,6,2,
		4,0,3, 4,3,7,
		3,2,6, 3,6,7,
		5,1,0, 4,5,0
	};

	static uint my_indices = 0;
	static uint num_indices = 36;
	glGenBuffers(1, (GLuint*)&(my_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)* num_indices, indices, GL_STATIC_DRAW);
	glVertexPointer(3, GL_FLOAT, 0, vertices);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
    //------------------------------------------------------------------------------------------------------------------------------------------------------

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_GL_SwapWindow(App->window->window);
	
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	App->editor->AddLog("Destroying 3D Renderer\n");

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(App->window->window);
	SDL_Quit();

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}