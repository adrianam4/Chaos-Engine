#pragma once
#include "Globals.h"
#include "Module.h"
#include "glmath.h"
#include <vector>

#include "Component.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(ComponentType type, const char* path, bool isDropped);
	~ComponentMaterial();

	bool loadTexture(const char* _path);
	void Enable() override;
	void Update() override;
	void Disable() override;
	void OnEditor(int i) override;

private:
	int width;
	int height;
	const char* path;
	ILuint myImageId;
	bool showCheckerTexture;
	GLubyte checkerImageData[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	const char* checkerTextPath;
	char* oldTextPath;
	bool isDropped;
};