#pragma once
#include"Mesh.h"
#include"Globals.h"
#include"physfs.h"
#include "assimp/cimport.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postProcess.h"
class GameObject;
class theBuffer {
public:
	theBuffer(char* THbuffer, int THsize) {
		buffer = THbuffer;
		size = THsize;
	}
	theBuffer(){}
	char* buffer;
	int size;
	std::string libraryDir;
	float3 position;
	float3 Rotation;
	float3 scale;
};

class FBXimporter {
public:
	FBXimporter();
	~FBXimporter();
	std::vector<theBuffer*>* saveToOurFile(const char* originPath, const char* destinationPath);
	void SpecialsaveToOurFile(const char* originPath, const char* destinationPath, GameObject* object);
	bool saveMesh(const char* destinationPath, Mesh* object);
	Mesh* readFile(const char* path);
	Mesh* getNewMeshFBX(char* data);
	void SpecialreadFromFBX(const char* originPath, const char* destiantion);
	std::vector<theBuffer*>* loadFromOurFile(const char* originPath, const char* UID, const char* mesh, const char* Numbermesh, const char* extension);
private:
	float3 FromQuatToEuler(Quat quatAngles);
	theBuffer* procesOneOfOurFile(char* data,int size);
	theBuffer* ProcessMesh(aiMesh* mesh, const aiScene* scene, int numOf);
	void ProcessNode(aiNode* node, const aiScene* scene);
	GameObject* SpecialProcessNode(aiNode* node, const aiScene* scene);
	void readFromFBX(const char* originPath);
	
	bool saveInFile(const char* path, void* data, int size);
	char* assingToBufferFBX(char* buffer, void* data,int *arrayPos,int dataSize);
	int getFileSize(const char* path, PHYSFS_File* file);
	
	std::vector<theBuffer*>bufferArray;
};
