#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "ModuleLoadGeometry.h"

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);
	void DrawMesh(LoadGeometry *geometryMesh, uint *VAO);
	void InitMesh(LoadGeometry *geometryMesh, const char* path, uint *VAO ,uint *bufferIndices, uint *bufferVertex);
public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	LoadGeometry *mesh;
	LoadGeometry *mesh2;

	//Tree
	uint VAO;
	uint bufferIndices;
	uint bufferVertex;

	//Cat
	uint VAO2;
	uint bufferIndices2;
	uint bufferVertex2;
};