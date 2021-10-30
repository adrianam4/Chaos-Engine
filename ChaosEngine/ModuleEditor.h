#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitives.h"
#include "GameObject.h"

#include "imgui.h"

class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool startEnabled = true);
	~ModuleEditor();

	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	void SaveConfig();
	void LoadConfig();
	void ComproveScreen();

	void AddLog(const char* fmt, ...);
	void DOptionsmenu(ComponentType type);
	void AddCube(float3 pos, float3 sca);
	void AddPyramid(float3 pos, float3 sca);
	void AddSphere(float3 pos, float3 sca);
	void AddCylinder(float3 pos);
	void AddPlane(float3 pos, float3 sca);
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
	ComponentType showOptionsMenu;
	float3 position;
	float3 M;
	void childrenManage(int i);
	
public:
	ImGuiTextBuffer consoleBuffer;
	bool wireframe;
	bool normals;
	bool showAABB;
	bool scrollToBottom;
	GameObject* objectSelected;

	std::vector<MyCube*> cubes;
	std::vector<MyCylinder*> cylinders;
	std::vector<MySphere*> spheres;
	std::vector<MyPyramid*> pyramids;
	std::vector<MyPlane3D*> planes;
	GameObject* objectToChildren;
	
	int lastId;
	bool isSelected;
};