#include "Globals.h"
#include "Application.h"
#include "ModuleLoadGeometry.h"
#include "ModuleEditor.h"

#include "Assimp/include/assimp/cimport.h"
#include "Assimp/include/assimp/scene.h"
#include "Assimp/include/assimp/postprocess.h"
#pragma comment (lib, "Assimp/x86-Release/assimp-vc142-mt.lib")


LoadGeometry::LoadGeometry(Application* app, bool start_enabled)
{}

LoadGeometry::~LoadGeometry()
{
	LOGCE("Unloading geometry\n");
	aiDetachAllLogStreams();
}

void LoadGeometry::LoadFile(const char* file_path)
{

	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[i];
			ourMesh.num_vertex = mesh->mNumVertices;
			ourMesh.vertex = new float[mesh->mNumVertices * 3];
			memcpy(ourMesh.vertex, mesh->mVertices, sizeof(float) * mesh->mNumVertices * 3);
			LOGCE("New mesh with %d vertices", mesh->mNumVertices);

			if (mesh->HasFaces())
			{
				ourMesh.num_index = mesh->mNumFaces * 3;
				ourMesh.index = new uint[ourMesh.num_index];
				for (uint i = 0; i < mesh->mNumFaces; ++i)
				{
					aiFace& face = mesh->mFaces[i];
					if (mesh->mFaces[i].mNumIndices != 3)
					{
						LOGCE("WARNING, geometry face with != 3 indices!");
					}
					else
						memcpy(&ourMesh.index[i * 3], face.mIndices, 3 * sizeof(uint));
				}
			}
		}
		aiReleaseImport(scene);
	}
	else
	LOGCE("Error loading scene % s", file_path);
}