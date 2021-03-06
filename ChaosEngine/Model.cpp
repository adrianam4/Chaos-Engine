#include "Model.h"

Model::Model()
{
}

Model::Model(Mesh* mesh)
{
	meshes.push_back(*mesh);
}
Model::Model(char* path, GameObject* lastObj)
{
	LoadModel(path, lastObj);
}

void Model::Draw(float* matrix)
{
	for (unsigned int i = 0; i < meshes.size(); ++i)
	{
		meshes[i].Draw(matrix);
	}
	//App->editor->AddLog("Drawing Model\n");
}
Model::Model(theBuffer* theArray) {
	meshes.push_back(createMesh(theArray->buffer));
}
Model::Model(std::vector<theBuffer*>* theArray) 
{
	for (int a = 0; a < theArray->size(); a++) 
	{
		char* pointer = (*theArray)[a]->buffer;
		meshes.push_back(createMesh(pointer));
	}
}

Model::Model(std::vector<uint> indices, std::vector<Vertex> vertices, std::vector<Textures> textures, std::vector<float> texCoords)
{
	meshes.push_back(Mesh(vertices, indices, textures, texCoords));
}
Mesh Model::createSpecialMesh(char* buffer) {
	FBXimporter i;
	return (*i.getNewMeshFBX(buffer));
}
Mesh Model::createMesh(char* buffer) 
{
	FBXimporter i;
	return (*i.getNewMeshFBX(buffer));
}

void Model::LoadModel(std::string path, GameObject* lastObj)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_SplitLargeMeshes | aiProcess_OptimizeMeshes);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));
	ProcessNode(scene->mRootNode, scene, lastObj);
	App->editor->AddLog("Loading Model\n");
}

void Model::ProcessNode(aiNode* node, const aiScene* scene, GameObject* lastObj)
{
	// process all the node's meshes
	for (int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}
	//then do te same for each of it's children
	for (int i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], scene, lastObj);
	}

	aiVector3D translation;
	aiVector3D scale;
	aiQuaternion rotation;

	node->mTransformation.Decompose(scale, rotation, translation);

	//lastObj->rot = rotation;
	//lastObj->trans = translation;
	//lastObj->sca = scale;

	App->scene->gameObjects[App->scene->gameObjects.size()-1]->rot.w = rotation.w;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->rot.x = rotation.x;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->rot.y = rotation.y;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->rot.z = rotation.z;

	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->sca.x = scale.x;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->sca.y = scale.y;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->sca.z = scale.z;

	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->trans.x = translation.x;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->trans.y = translation.y;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->trans.z = translation.z;
	App->editor->AddLog("Processing Node\n");
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Textures> textures;
	std::vector<float> texCoords;

	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex;
		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

		vertex.normal.x = mesh->mNormals[i].x;
		vertex.normal.y = mesh->mNormals[i].y;
		vertex.normal.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0])
		{
			texCoords.push_back(mesh->mTextureCoords[0][i].x);
			texCoords.push_back(mesh->mTextureCoords[0][i].y);
		}
		else
		{
			texCoords.push_back(0); //x
			texCoords.push_back(0); //y
		}
		vertices.push_back(vertex);
	}
	numFaces += mesh->mNumFaces;

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	//if (mesh->mMaterialIndex >= 0)
	//{
	//	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	//	std::vector<Textures> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	//	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	//	std::vector<Textures> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	//	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	//}
	App->editor->AddLog("Processing Mesh\n");
	return Mesh(vertices, indices, textures, texCoords);
}

//std::vector<Textures> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
//{
//	std::vector<Textures> textures;
//	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
//	{
//		aiString str;
//		mat->GetTexture(type, i, &str);
//		Texture texture;
//		texture.id = TextureFromFile(str.C_Str(), directory);
//		texture.type = typeName;
//		texture.path = str;
//		textures.push_back(texture);
//	}
//	return textures;
//}