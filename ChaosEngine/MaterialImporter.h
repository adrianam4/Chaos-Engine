#pragma once

#include"Globals.h"
#include"physfs.h"

class MaterialImporter 
{
public:
	MaterialImporter();
	~MaterialImporter();

	bool SaveMaterial(const char* destinationPath, const char* sourcePath);
	const char* ReadFile(const char* path);
private:
	bool SaveInFile(const char* path, void* data, int size);
	char* AssingToBuffer(char* buffer, void* data, int* arrayPos, int dataSize);
	int GetFileSize(const char* path, PHYSFS_File* file);
	const char* GetNewMaterial(char* data);
};
