#pragma once

#include "Globals.h"
#include "Application.h"
#include "Mesh.h"

#include "assimp/cimport.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postProcess.h"

#include <vector>

class Model
{
public:
	Model() {}
	Model(char* path)
	{
		LoadModel(path);
	}
	void Draw();
private:
	//std::vector<Textures> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory; // or const char*

	void LoadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	//std::vector<Textures> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};