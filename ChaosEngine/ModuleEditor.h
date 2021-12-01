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
	//update_status PreUpdate(float dt);
	update_status Update(float dt)override;
	update_status PostUpdate(float dt)override;
	update_status PreUpdate(float dt)override;
	bool CleanUp();
	void SaveConfig();
	void LoadConfig();
	void LoadScene();
	void SaveScene();
	void ComproveScreen();

	//Docking Helper functions
	bool DockingRootItem(char* id, ImGuiWindowFlags winFlags);
	void BeginDock(char* dockSpaceId, ImGuiDockNodeFlags dockFlags, ImVec2 size = { .0f, .0f });

	void AddLog(const char* fmt, ...);
	void DOptionsmenu(ComponentType type);
	void AddCube(float3 pos, float3 sca);
	void AddPyramid(float3 pos, float3 sca);
	void AddSphere(float3 pos, float3 sca);
	void AddCylinder(float3 pos, float3 sca);
	void AddPlane(float3 pos, float3 sca);
	void DrawPrimitives();
	void UpdateAll();

private:
	uint guizmoType;
	int maxFPS;
	int width = 1280;
	int height = 720;
	float brightness;
	bool fullscreen = false;
	bool resizable = true;
	bool borderless = false;
	bool dekstop = false;
	
	ComponentType showOptionsMenu;
	float3 position;
	float3 M;
	float3 R;
	void childrenManage(int i);

public:
	Grid* grid;
	ImGuiTextBuffer consoleBuffer;
	ImGuiWindowFlags sceneWindow = 0;
	ImVec2 lastViewportSize;
	bool showAABB;
	bool scrollToBottom;
	GameObject* objectSelected;
	float4 viewport;

	std::vector<MyCube*> cubes;
	std::vector<MyCylinder*> cylinders;
	std::vector<MySphere*> spheres;
	std::vector<MyPyramid*> pyramids;
	std::vector<MyPlane*> planes;
	GameObject* objectToChildren;

	int lastId;
	bool isSelected;
	bool start = true;
	bool stop = false;
	bool advance = false;

	//Models
	u32 houseModelId;
	u32 penguinModelId;
	u32 carModelId;
	//Materials
	u32 houseMaterialId;
	u32 penguinMaterialId;
	u32 carMaterialId;
	//Icons
	u32 folderId;
	u32 playId;
	u32 stopId;
	u32 pauseId;
	u32 advanceId;
	u32 speedUpId;
	u32 speedDownId;
	u32 backId;

	ImTextureID folderIcon;
	ImTextureID playIcon;
	ImTextureID stopIcon;
	ImTextureID pauseIcon;
	ImTextureID advanceIcon;
	ImTextureID speedUpIcon;
	ImTextureID speedDownIcon;
	ImTextureID backIcon;
};