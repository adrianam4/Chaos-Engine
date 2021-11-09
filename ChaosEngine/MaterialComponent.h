#pragma once
#include "Globals.h"
#include "Module.h"
#include "glmath.h"
#include <vector>

#include "Component.h"
#include "MaterialImporter.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(ComponentType type, const char* path, bool isDropped);
	~ComponentMaterial();

	void Enable() override;
	void Update() override;
	void Disable() override;
	void OnEditor(int i) override;
	void Load(const char* path) override;
	void Save(const char* path) override;

private:
	MaterialImporter importer;
	ILuint myImageId;
	bool showCheckerTexture;
	GLubyte checkerImageData[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	const char* checkerTextPath;
	char* oldTextPath;
	bool isDropped;
};