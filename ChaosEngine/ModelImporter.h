#pragma once

#include"Mesh.h"
#include"Globals.h"
#include"physfs.h"

class FBXImporter 
{
public:
	FBXImporter();
	~FBXImporter();

	bool saveMesh(const char* destinationPath, Mesh* object);
	Mesh* readFile(const char* path);
private:
	bool saveInFile(const char* path, void* data, int size);
	char* assingToBuffer(char* buffer, void* data,int *arrayPos,int dataSize);
	int getFileSize(const char* path, PHYSFS_File* file);
	Mesh* getNewMesh(char* data);
};
