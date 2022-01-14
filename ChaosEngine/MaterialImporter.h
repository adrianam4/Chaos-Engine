#pragma once

#include"Globals.h"
#include"physfs.h"

class ResourceMatertial;

class MaterialImporter 
{
public:
	MaterialImporter();
	~MaterialImporter();

	void SaveMaterial(std::string sourcePath, std::string compression);
	std::vector<int> ImportMaterial(std::string sourcePath, bool isDropped, ResourceMatertial* resource);
	std::vector<int> LoadMaterial(std::string sourcePath, bool isDropped);
	std::vector<int> GetMaterialData();
	std::string ddsPath;

public:
	int w;
	int h;
	ILuint imageId;
	GLuint textId;
	ILubyte* data;
};
