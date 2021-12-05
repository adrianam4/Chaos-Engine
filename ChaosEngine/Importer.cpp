#include"Importer.h"
#include<stack>
#include"Application.h"
#include "Resource.h"
#include "ResourceMesh.h"
#include"TransformComponent.h"

FBXimporter::FBXimporter() {}
FBXimporter::~FBXimporter() {
	for (int a = 0; a < bufferArray.size(); a++) {
		delete[] bufferArray[a]->buffer;
		delete bufferArray[a];
	}
	bufferArray.clear();
}
theBuffer* SpecialsaveToOurFile(const char* originPath, const char* destinationPath) {
	return nullptr;
}
std::vector<theBuffer*>* FBXimporter::saveToOurFile(const char* originPath, const char* destinationPath, Resource* resource) 
{
	//////////////////////////////////////////// read the fbx file and save de data in buffers ////////////////////////////////////////////
	readFromFBX(originPath,resource);


	//////////////////////////////////////////// write the buffers data in our custom file format ////////////////////////////////////////////	
	std::string path = destinationPath;
	LCG uidGenerator;
	std::string i;
	int UID = 0;
	for (int a = 0; a < 11; a++) {
		
		i += std::to_string(uidGenerator.IntFast());
		if (i.size() > 8) 
		{
			int v = i.size()-8;
			i=i.substr(0, 8);
			break;
		}
		else if (i.size() == 8) 
		{
			break;
		}
	}
	for (int a = 0; a < bufferArray.size(); a++) {
		path = path + i;//generate UID
		path = path + std::to_string(a);
		path = path + std::to_string(bufferArray.size());
		path = path + ".msh";
		saveInFile(path.c_str(), bufferArray[a]->buffer, bufferArray[a]->size);
		bufferArray[a]->libraryDir = path;
		path.clear();
		path = destinationPath;
	}
	return &bufferArray;
}

void  FBXimporter::SpecialreadFromFBX(const char* originPath, Resource* resource) {
	
	//unsigned int flags = 0;
	//ResourceMesh* res = (ResourceMesh*)resource;

	//if (res->metaData.joinVertex)
	//	flags |= aiProcess_JoinIdenticalVertices;
	//if (res->metaData.triangulate)
	//	flags |= aiProcess_Triangulate;
	//if (res->metaData.generateNormals)
	//	flags |= aiProcess_GenNormals;
	//if (res->metaData.generateSmoothNormals)
	//	flags |= aiProcess_GenSmoothNormals;
	//if (res->metaData.removeMaterials)
	//	flags |= aiProcess_RemoveRedundantMaterials;
	//if (res->metaData.infacingNormals)
	//	flags |= aiProcess_FixInfacingNormals;
	//if (res->metaData.genUvCoords)
	//	flags |= aiProcess_GenUVCoords;
	//if (res->metaData.transUvCoords)
	//	flags |= aiProcess_TransformUVCoords;
	//if (res->metaData.findInstances)
	//	flags |= aiProcess_FindInstances;
	//if (res->metaData.optimizeMesh)
	//	flags |= aiProcess_OptimizeMeshes;
	//if (res->metaData.flipUvs)
	//	flags |= aiProcess_FlipUVs;

	// aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_OptimizeMeshes
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(originPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_OptimizeMeshes);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}


	SpecialProcessNode(scene->mRootNode, scene);


}
void FBXimporter::SpecialsaveToOurFile(const char* originPath, const char* destinationPath, GameObject* object)
{
	std::stack<GameObject*>stackNode;
	GameObject* theObject;
	for (int i = 0; i < App->scene->gameObjects.size(); i++)
	{
		for (int a = 0; a < App->scene->gameObjects[i]->childrens.size(); a++)
		{
			stackNode.push(App->scene->gameObjects[i]->childrens[a]);
		}
		while (!stackNode.empty())
		{
			theObject = stackNode.top();
			stackNode.pop();
			if (theObject->buffer.size > 0)
			{
				LCG uidGenerator;
				std::string i;
				int UID = 0;
				for (int a = 0; a < 11; a++) {

					i += std::to_string(uidGenerator.IntFast());
					if (i.size() > 8) {
						int v = i.size() - 8;
						i = i.substr(0, 8);
						break;
					}
					else if (i.size() == 8) {
						break;
					}
				}
				std::string sourcePath = destinationPath;
				//name 
				std::string finalPath = destinationPath + i + ".msh";
				saveInFile(finalPath.c_str(), theObject->buffer.buffer, theObject->buffer.size);
				for (int q = 0; q < theObject->components.size(); q++) {
					if (theObject->SearchComponent(theObject, ComponentType::MESH) != -1) {
						theObject->components[q]->modelPath = finalPath.c_str();
						break;
					}
				}
			}

			for (unsigned i = 0; i < theObject->childrens.size(); ++i)
			{
				stackNode.push(theObject->childrens[i]);
			}
		}
	}


}
void  FBXimporter::SpecialreadFromFBX(const char* originPath, const char* destinationPath, Resource* resource) 
{
	//unsigned int flags = 0;
	//ResourceMesh* res = (ResourceMesh*)resource;

	//if (res->metaData.joinVertex)
	//	flags |= aiProcess_JoinIdenticalVertices;
	//if (res->metaData.triangulate)
	//	flags |= aiProcess_Triangulate;
	//if (res->metaData.generateNormals)
	//	flags |= aiProcess_GenNormals;
	//if (res->metaData.generateSmoothNormals)
	//	flags |= aiProcess_GenSmoothNormals;
	//if (res->metaData.removeMaterials)
	//	flags |= aiProcess_RemoveRedundantMaterials;
	//if (res->metaData.infacingNormals)
	//	flags |= aiProcess_FixInfacingNormals;
	//if (res->metaData.genUvCoords)
	//	flags |= aiProcess_GenUVCoords;
	//if (res->metaData.transUvCoords)
	//	flags |= aiProcess_TransformUVCoords;
	//if (res->metaData.findInstances)
	//	flags |= aiProcess_FindInstances;
	//if (res->metaData.optimizeMesh)
	//	flags |= aiProcess_OptimizeMeshes;
	//if (res->metaData.flipUvs)
	//	flags |= aiProcess_FlipUVs;

	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(originPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_OptimizeMeshes);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}

	SpecialProcessNode(scene->mRootNode, scene);

	SpecialsaveToOurFile(originPath, destinationPath, App->scene->gameObjects[App->scene->gameObjects.size()-1]);

}


void FBXimporter::SpecialProcessNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes
	aiVector3D translation = { 0,0,0 };
	aiVector3D scale = { 1,1,1 };
	aiQuaternion rotation = { 1,0,0,0 };

	bool toAABB=false;

	aiVector3D Auxtranslation = { 0,0,0 };
	aiVector3D Auxscale = { 1,1,1 };
	aiQuaternion Auxrotation = { 1,0,0,0 };
	std::stack<aiNode*> stackNode;
	std::stack<GameObject*> stackParent;
	GameObject* aux = new GameObject();
	for (int a = 0; a < node->mNumChildren; a++) {
		stackNode.push(node->mChildren[a]);


		stackParent.push(aux);
	}
	while (!stackNode.empty())
	{
		aiNode* Thenode = stackNode.top();
		stackNode.pop();
		GameObject* aux = App->scene->CreateGameObject(false, 1, "sfe");

		GameObject* parent = stackParent.top();
		stackParent.pop();
		parent->childrens.push_back(aux);
		for (unsigned i = 0; i < Thenode->mNumChildren; ++i)
		{
			stackNode.push(Thenode->mChildren[i]);
			stackParent.push(aux);
		}




		unsigned nMeshes = Thenode->mNumMeshes;




		Thenode->mTransformation.Decompose(scale, rotation, translation);



		aux->buffer.position.x = translation.x;
		aux->buffer.position.y = translation.y;
		aux->buffer.position.z = translation.z;
		Quat q(rotation.x, rotation.y, rotation.z, rotation.w);
		aux->buffer.Rotation = FromQuatToEuler(q);
		aux->name=Thenode->mName.C_Str();

		Component* e;
		

		

		for (int q = 0; q < nMeshes; q++) {
			aiMesh* mesh = scene->mMeshes[Thenode->mMeshes[q]];
			theBuffer* temporal = ProcessMesh(mesh, scene, Thenode->mNumMeshes);

			e = aux->CreateOneMeshComponent(temporal, "hsjbvjh");
			aux->isImported = true;
			aux->components.push_back(e);
			e->owner = aux;
			toAABB = true;
			/*e=aux->CreateComponent(ComponentType::MATERIAL, aux->name.c_str(), true);

			e->owner = aux;*/

			aux->buffer.buffer = temporal->buffer;
			aux->buffer.size = temporal->size;
			
		}
		e = new ComponentTransform(aux, aux->buffer.position, float3(1, 1, 1), aux->buffer.Rotation);
		//e = new ComponentTransform(&aux->buffer.position, &float3(1, 1, 1), &aux->buffer.Rotation, "hh");
		//e = aux->CreateComponent(ComponentType::TRANSFORM, &aux->buffer.position, &float3(1, 1, 1), &aux->buffer.Rotation);
		e->owner = aux;
		aux->components.push_back(e);
		e->setOwner();
		Thenode->mTransformation.Decompose(scale, rotation, translation);
		if (toAABB) {
			e->CreateAABB(ComponentType::MESH, aux,true);
			toAABB = false;
		}
		


	}
	//setTransform(aux);
	App->scene->gameObjects.push_back(aux);
	aux->isImported = true;
	App->editor->AddLog("Processing Node\n");
	
}
float3 FBXimporter::FromQuatToEuler(Quat quatAngles) {
	float3 angles;

	angles = quatAngles.ToEulerXYZ();

	angles.x = math::RadToDeg(angles.x);
	angles.y = math::RadToDeg(angles.y);
	angles.z = math::RadToDeg(angles.z);

	return angles;
}
void  FBXimporter::readFromFBX(const char* originPath, Resource* resource) 
{
	Assimp::Importer import;

	unsigned int flags = 0;
	ResourceMesh* res = (ResourceMesh*)resource;

	if (res->metaData.joinVertex)
		flags |= aiProcess_JoinIdenticalVertices;
	if (res->metaData.triangulate)
		flags |= aiProcess_Triangulate;
	if (res->metaData.generateNormals)
		flags |= aiProcess_GenNormals;
	if (res->metaData.generateSmoothNormals)
		flags |= aiProcess_GenSmoothNormals;
	if (res->metaData.removeMaterials)
		flags |= aiProcess_RemoveRedundantMaterials;
	if (res->metaData.infacingNormals)
		flags |= aiProcess_FixInfacingNormals;
	if (res->metaData.genUvCoords)
		flags |= aiProcess_GenUVCoords;
	if (res->metaData.transUvCoords)
		flags |= aiProcess_TransformUVCoords;
	if (res->metaData.findInstances)
		flags |= aiProcess_FindInstances;
	if (res->metaData.optimizeMesh)
		flags |= aiProcess_OptimizeMeshes;
	if (res->metaData.flipUvs)
		flags |= aiProcess_FlipUVs;

	const aiScene* scene = import.ReadFile(originPath, flags);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}

	ProcessNode(scene->mRootNode, scene);
}
std::vector<theBuffer*>* FBXimporter::loadFromOurFile(const char* originPath, const char* UID, const char* mesh, const char* Numbermesh, const char* extension)
{
	std::string first = originPath;
	std::string Mesh = mesh;
	std::string Uid = UID;
	std::string NumMesh = Numbermesh;
	std::string last = extension;
	first = first + Uid;
	first = first + Mesh;
	first = first + NumMesh;
	first = first + last;
	PHYSFS_File* file = PHYSFS_openRead(first.c_str());
	if (file == nullptr) {
		App->editor->AddLog("Error openenig the file %s\n", first.c_str());
		return NULL;
	}
	int size = getFileSize(first.c_str(), file);
	if (size == -1) {
		App->editor->AddLog("the file cannot be opened for reading\n");
		return NULL;
	}
	char* bufferData = new char[size];

	PHYSFS_read(file, bufferData, size, 1);
	int numMeshes = std::stoi(NumMesh);
	bufferArray.push_back(procesOneOfOurFile(bufferData, size));
	PHYSFS_close(file);
	delete[] bufferData;
	bufferData = nullptr;
	for (int a = 1; a < numMeshes; a++) 
	{
		int theMesh = std::stoi(Mesh);
		theMesh++;
		first = originPath;
		Mesh = std::to_string(theMesh);
		first = first + Uid;
		first = first + Mesh;
		first = first + NumMesh;
		first = first + last;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		file = nullptr;
		file = PHYSFS_openRead(first.c_str());
		if (file == nullptr) 
		{
			App->editor->AddLog("Error openenig the file %s\n", first.c_str());
			return NULL;
		}
		int size = getFileSize(first.c_str(), file);
		if (size == -1) 
		{
			App->editor->AddLog("the file cannot be opened for reading\n");
			return NULL;
		}
		bufferData = new char[size];

		PHYSFS_read(file, bufferData, size, 1);

		bufferArray.push_back(procesOneOfOurFile(bufferData, size));
		PHYSFS_close(file);
		delete[] bufferData;
		bufferData = nullptr;
	}
	return &bufferArray;
	
}

theBuffer* FBXimporter::procesOneOfOurFile(char* data,int size) {
	
	int numVertex;
	int numIndex;
	int pointer = 0;
	int numTextCoord;
	char* buffer = new char[size];
	int numMeshes;


	memcpy(&numVertex, &data[pointer], 1 * sizeof(int));
	
	memcpy(&buffer[pointer], &data[pointer], 1 * sizeof(int));
	pointer += 1 * sizeof(int);


	//buffer = assingToBuffer(buffer, &data, &pos, 1 * sizeof(int));
	memcpy(&numIndex, &data[pointer], 1 * sizeof(int));
	memcpy(&buffer[pointer], &data[pointer], 1 * sizeof(int));
	pointer += 1 * sizeof(int);


	//buffer = assingToBuffer(buffer, &data, &pos, 1 * sizeof(int));
	memcpy(&numTextCoord, &data[pointer], 1 * sizeof(int));
	memcpy(&buffer[pointer], &data[pointer], 1 * sizeof(int));
	pointer += 1 * sizeof(int);
	//buffer = assingToBuffer(buffer, &data, &pos, 1 * sizeof(int));


	
	memcpy(&buffer[pointer], &data[pointer], sizeof(float) * numVertex);
	pointer += sizeof(float) * numVertex;
	
	
	memcpy(&buffer[pointer], &data[pointer], sizeof(int) * numIndex);
	pointer += sizeof(int) * numIndex;


	memcpy(&buffer[pointer], &data[pointer], sizeof(float) * numTextCoord);
	pointer += sizeof(float) * numTextCoord;

	theBuffer* buff = new theBuffer(buffer,pointer);
	return buff;
}
void FBXimporter::ProcessNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes
	
	for (int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		bufferArray.push_back(ProcessMesh(mesh, scene, node->mNumMeshes));
	}
	//then do te same for each of it's children
	for (int i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], scene);
	}

	aiVector3D translation;
	aiVector3D scale;
	aiQuaternion rotation;

	node->mTransformation.Decompose(scale, rotation, translation);
	
	//lastObj->rot = rotation;
	//lastObj->trans = translation;
	//lastObj->sca = scale;

	//App->scene->gameObjects[App->scene->gameObjects.size() - 1]->rot.w = rotation.w;
	//App->scene->gameObjects[App->scene->gameObjects.size() - 1]->rot.x = rotation.x;
	//App->scene->gameObjects[App->scene->gameObjects.size() - 1]->rot.y = rotation.y;
	//App->scene->gameObjects[App->scene->gameObjects.size() - 1]->rot.z = rotation.z;

	//App->scene->gameObjects[App->scene->gameObjects.size() - 1]->sca.x = scale.x;
	//App->scene->gameObjects[App->scene->gameObjects.size() - 1]->sca.y = scale.y;
	//App->scene->gameObjects[App->scene->gameObjects.size() - 1]->sca.z = scale.z;

	//App->scene->gameObjects[App->scene->gameObjects.size() - 1]->trans.x = translation.x;
	//App->scene->gameObjects[App->scene->gameObjects.size() - 1]->trans.y = translation.y;
	//App->scene->gameObjects[App->scene->gameObjects.size() - 1]->trans.z = translation.z;
	App->editor->AddLog("Processing Node\n");
}
theBuffer* FBXimporter::ProcessMesh(aiMesh* mesh, const aiScene* scene,int numOfMesh)
{
	unsigned currentSize = 0u;
	int pos = 0;
	int bufferSize = 0;
	char* buffer = NULL;
	int* lastSize;


	int numOf = mesh->mNumVertices*3;
	buffer = assingToBufferFBX(buffer, &numOf, &pos, 1 * sizeof(int));
	int ind;
	int a = 0;
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
		{
			a++;
		}
	}
	int* indexAux = new int[a];
	float* textureAux = new float[mesh->mNumVertices * 2];
	numOf = a;
	ind = a;
	buffer = assingToBufferFBX(buffer, &numOf, &pos, 1 * sizeof(int));

	numOf = mesh->mNumVertices * 2;
	buffer = assingToBufferFBX(buffer, &numOf, &pos, 1 * sizeof(int));

	///////////////////////////////////////////////////////////////////// VERTEX /////////////////////////////////////////////////////////////////////
	float* vertAux = new float[3 * mesh->mNumVertices];

	a = 0;
	int b = 0;
	while( a < mesh->mNumVertices*3)
	{
		
		vertAux [a]= mesh->mVertices[b].x;
		a++;
		vertAux[a] = mesh->mVertices[b].y;
		a++;
		vertAux[a] = mesh->mVertices[b].z;
		a++;
		b++;
	}
	buffer = assingToBufferFBX(buffer, vertAux, &pos, 3 * mesh->mNumVertices * sizeof(float));
	///////////////////////////////////////////////////////////////////// INDEX /////////////////////////////////////////////////////////////////////
	a = 0;
	b = 0;
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
		{
			indexAux[a]=face.mIndices[j];
			a++;
		}
		
	}

	buffer = assingToBufferFBX(buffer, indexAux, &pos, ind * sizeof(int));

	///////////////////////////////////////////////////////////////////// TEXTURES /////////////////////////////////////////////////////////////////////
	a = 0;
	b = 0;
	while (a < mesh->mNumVertices)
	{
		if (mesh->mTextureCoords[0])
		{
			textureAux[b] = mesh->mTextureCoords[0][a].x;
			b++;
			textureAux[b] = mesh->mTextureCoords[0][a].y;
			b++;
		}
		else
		{

			textureAux[b] = 0.0f;//x
			b++;
			textureAux[b] = 0.0f; //y
			b++;
		}
		a++;
	}
	buffer = assingToBufferFBX(buffer, textureAux, &pos, mesh->mNumVertices * 2 * sizeof(float));
	delete[] vertAux;
	delete[] indexAux;
	delete[] textureAux;
	
	
	App->editor->AddLog("Processing Mesh\n");
	theBuffer* buff = new theBuffer(buffer, pos);
	return buff;
}
bool FBXimporter::saveMesh(const char* destinationPath, Mesh* object) 
{
	unsigned currentSize=0u;
	int pos = 0;
	int bufferSize=0;
	char* buffer=NULL;
	int* lastSize;
	float* vertAux = new float[3 * object->vertices.size()];
	int a = 0;
	int b=0;
	while(a <  object->vertices.size()) {
		vertAux[b]=object->vertices[a].position.x;
		b++;
		vertAux[b] = object->vertices[a].position.y;
		b++;
		vertAux[b] = object->vertices[a].position.z;
		b++;
		a++;
	}
	int* indexAux = new int[object->indices.size()];
	a = 0;
	
	while (a < object->indices.size()) {
		indexAux[a] = object->indices[a];
		a++;
	}
	
	float* textureAux = new float[object->texCoords.size()];
	a = 0;
	while (a < object->texCoords.size()) {
		textureAux[a] = object->texCoords[a];
		a++;
	}
	

	int numOf = object->vertices.size();
	buffer=assingToBufferFBX(buffer, &numOf, &pos,1 * sizeof(int));
	
	
	numOf = object->indices.size();
	buffer = assingToBufferFBX(buffer, &numOf,  &pos,1 * sizeof(int));

	numOf = object->texCoords.size();
	buffer = assingToBufferFBX(buffer, &numOf, &pos,1 * sizeof(int));

	
	buffer = assingToBufferFBX(buffer, vertAux, &pos, 3 * object->vertices.size() * sizeof(float));

	buffer = assingToBufferFBX(buffer, indexAux, &pos, object->indices.size() * sizeof(int));

	buffer = assingToBufferFBX(buffer, textureAux, &pos, object->texCoords.size()*sizeof(float));

	
	delete[] vertAux;
	delete[] indexAux;
	delete[] textureAux;
	//assingToBuffer(buffer, &object->vertices[0], sizeof(Vertex)*3* object->vertices.size(), currentSize,&pos);
	saveInFile(destinationPath, buffer, pos);
	delete[] buffer;
	return true;
}
char* FBXimporter::assingToBufferFBX(char* buffer, void* data, int* arrayPos, int dataSize)
{


	char* newBuffer = new char[(*arrayPos) +dataSize];
	memcpy(newBuffer, buffer, (*arrayPos));
	
	memcpy(&newBuffer[(*arrayPos)], data, dataSize);
	if (buffer != nullptr) {
		delete[] buffer;
	}
	buffer = nullptr;



	(*arrayPos) += dataSize;
	return newBuffer;
}
bool FBXimporter::saveInFile(const char* path, void* data, int size) {
	
		//PHYSFS_setWriteDir(prefPath.c_str());
	
	PHYSFS_File* fileToWrite = PHYSFS_openWrite(path);
	if (fileToWrite == nullptr)
	{	
		App->editor->AddLog("Error writing in %s -> %s\n", path, PHYSFS_getLastError());		
		//PHYSFS_setWriteDir(".");	
		return false;
	}
	App->editor->AddLog("Mesh saved in file %s\n",path);
	PHYSFS_write(fileToWrite, data, size,1);
	PHYSFS_close(fileToWrite);

	return true;
}

int  FBXimporter::getFileSize(const char* path, PHYSFS_File* file) {
	
	if (file == nullptr) {
		App->editor->AddLog("Error reading in %s -> %s\n", path, PHYSFS_getLastError());
		return -1;
	}
	return PHYSFS_fileLength(file);
}
Mesh* FBXimporter::readFile(const char* path) {
	
	PHYSFS_File* file = PHYSFS_openRead(path);
	if (file == nullptr) {
		App->editor->AddLog("Error openenig the file %s\n", path);
		return NULL;
	}
	int size = getFileSize(path, file);
	if (size == -1) {
		App->editor->AddLog("the file cannot be opened for reading\n");
		return NULL;
	}
	char* bufferData = new char[size];
	PHYSFS_read(file, bufferData, size, 1);
	PHYSFS_close(file);		
	return getNewMeshFBX(bufferData);
}
Mesh* FBXimporter::getNewMeshFBX(char* data) {
	int pointer=0;
	int numVertex;
	int numIndex;
	int numTextCoord;
	std::vector<Textures> textures;

	memcpy(&numVertex, &data[pointer], 1*sizeof(int));
	pointer += (1 * sizeof(int));

	memcpy(&numIndex, &data[pointer], 1 * sizeof(int));
	pointer += (1 * sizeof(int));

	memcpy(&numTextCoord, &data[pointer], 1 * sizeof(int));
	pointer += (1 * sizeof(int));

	
	float* auxVertex = new float[numVertex];

	memcpy(auxVertex, &data[pointer],  sizeof(float)* numVertex);
	pointer += (sizeof(float) * numVertex);


	int* auxIndex = new int[numIndex];

	memcpy(auxIndex, &data[pointer],sizeof(int) * numIndex);
	pointer += (sizeof(int) * numIndex);


	float* auxTextureCoor = new float[numTextCoord];

	memcpy(auxTextureCoor, &data[pointer], sizeof(float) * numTextCoord);
	pointer += (sizeof(float) * numTextCoord);


	std::vector<Vertex> vertices;
	int i = 0;
	while (i < numVertex)
	{
		Vertex vertex;
		vertex.position.x = auxVertex[i];
		i++;
		vertex.position.y = auxVertex[i];
		i++;
		vertex.position.z = auxVertex[i];
		i++;

		
		vertices.push_back(vertex);
	}

	
	i = 0;
	std::vector<float> texCoords;
	while (i < numTextCoord)
	{
		texCoords.push_back(auxTextureCoor[i]);
		i++;
		texCoords.push_back(auxTextureCoor[i]);
		i++;
	}


	i = 0;
	std::vector<unsigned int> indices;
	while (i < numIndex)
	{
		indices.push_back(auxIndex[i]);
		i++;
	}

	delete[] auxVertex;
	delete[] auxIndex;
	delete[] auxTextureCoor;
	Mesh* newMesh = new Mesh(vertices,indices,textures, texCoords);
	return newMesh;
}
