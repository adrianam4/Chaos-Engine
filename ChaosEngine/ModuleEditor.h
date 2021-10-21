#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
#include "Primitives.h"
#include "GameObject.h"

#include "imgui.h"

class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	void SaveConfig();
	void LoadConfig();
	void ComproveScreen();

	void AddLog(const char* fmt, ...);

	void AddCube(float3 pos, float3 sca);
	void AddPyramid(float3 pos, float3 sca);
	void AddSphere(float3 pos, float3 sca, float radius, uint rings, uint sectors);
	void AddCylinder(float3 pos, float3 sca);
private:
	int maxFPS;
	int width;
	int height;
	float brightness;
	bool fullscreen = false;
	bool resizable = true;
	bool borderless = false;
	bool dekstop = false;
	MyPlane* plane;
public:
	ImGuiTextBuffer consoleBuffer;
	bool wireframe;
	bool normals;
	bool scrollToBottom;

	std::vector<MyCube*> cubes;
	std::vector<MyCylinder*> cylinders;
	std::vector<MySphere*> spheres;
	std::vector<MyPyramid*> pyramids;

	GameObject* objectSelected;
	int lastId;
	bool isSelected;
};