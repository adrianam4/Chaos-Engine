#pragma once

#include"Globals.h"
#include"physfs.h"

class MaterialImporter 
{
public:
	MaterialImporter();
	~MaterialImporter();

	void SaveMaterial(std::string sourcePath);
	std::vector<int> ImportMaterial(std::string sourcePath, bool isDropped);
	std::vector<int> LoadMaterial(std::string sourcePath, bool isDropped);
	std::vector<int> GetMaterialData();
	std::string ddsPath;

private:
	int w;
	int h;
	ILuint imageId;
	GLuint textId;
	ILubyte* data;
};
