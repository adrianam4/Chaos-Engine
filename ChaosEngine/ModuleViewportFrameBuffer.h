#pragma once
#include "Module.h"
#include "Globals.h"

#include <string>
class ComponentCamera;
class ModuleViewportFrameBuffer : public Module
{
public:
	ModuleViewportFrameBuffer(Application* app, bool start_enabled = true);
	~ModuleViewportFrameBuffer();

	bool Init() override;
	bool Start() override;
	update_status PreUpdate(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp();
	void pre(ComponentCamera* camera);
	void post(ComponentCamera* camera);
	void Resize(uint width, uint height, ComponentCamera* camera);
	void Bind(ComponentCamera* camera);
	void UnBind();
	void enableStart(ComponentCamera* camera);
public:
	
	
};