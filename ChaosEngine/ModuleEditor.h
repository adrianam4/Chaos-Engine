#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
#include "Primitives.h"

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
	MyCube* cube;
	MyPyramid* pyramid;
	//MyCylinder* cylinder;
	MySphere* sphere;
public:
	ImGuiTextBuffer consoleBuffer;
	bool wireframe;
	bool scrollToBottom;
};