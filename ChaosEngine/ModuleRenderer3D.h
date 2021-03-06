#pragma once
#include "Module.h"
#include "glmath.h"
#include "Light.h"

#define MAX_LIGHTS 8
class ComponentCamera;
class Model;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool startEnabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt) override;
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp();
	void InitModel(std::vector<theBuffer*>* theArray);
	void InitModel(theBuffer* theArray);
	void InitMesh(Mesh* mesh);
	void InitModel(std::vector<uint> indices, std::vector<Vertex> vertices, std::vector<Textures> textures, std::vector<float> texCoords);
	void InitModelPath(char *path);
	void OnResize(int width, int height);
	void InitMesh(char* path, GameObject* selectedObj);
	void DrawMeshes(ComponentCamera*editorCam);
	void DrawCameras();
	void DrawRay(LineSegment& myRay);
public:
	
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 normalMatrix;
	mat4x4 modelMatrix, viewMatrix, projectionMatrix;

	std::vector<Model> models;
};