#pragma once

#include"Globals.h"
#include"physfs.h"

class MaterialImporter 
{
public:
	MaterialImporter();
	~MaterialImporter();

	void SaveMaterial(const char* sourcePath);
	void ImportMaterial(std::string sourcePath, int* width, int* height, bool isDropped, ILuint* myImageId);
	void LoadMaterial(std::string sourcePath, int* width, int* height, bool isDropped, ILuint* myImageId);
	std::string ddsPath;
};
