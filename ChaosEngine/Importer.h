#pragma once
#include"Mesh.h"
#include"Globals.h"
#include"physfs.h"
#include "assimp/cimport.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postProcess.h"
class theBuffer {
public:
	theBuffer(char* THbuffer, int THsize) {
		buffer = THbuffer;
		size = THsize;
	}
	char* buffer;
	int size;
};
class FBXimporter {
public:
	FBXimporter();
	~FBXimporter();
	std::vector<theBuffer*>* saveToOurFile(const char* originPath, const char* destinationPath);
	bool saveMesh(const char* destinationPath, Mesh* object);
	Mesh* readFile(const char* path);
	Mesh* getNewMeshFBX(char* data);
	std::vector<theBuffer*>* loadFromOurFile(const char* originPath, const char* UID, const char* mesh, const char* Numbermesh, const char* extension);
private:
	theBuffer* procesOneOfOurFile(char* data,int size);
	theBuffer* ProcessMesh(aiMesh* mesh, const aiScene* scene, int numOf);
	void ProcessNode(aiNode* node, const aiScene* scene);
	void readFromFBX(const char* originPath);
	bool saveInFile(const char* path, void* data, int size);
	char* assingToBufferFBX(char* buffer, void* data,int *arrayPos,int dataSize);
	int getFileSize(const char* path, PHYSFS_File* file);
	
	std::vector<theBuffer*>bufferArray;
};
