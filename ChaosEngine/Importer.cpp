#include"Importer.h"

#include"Application.h"
FBXmporter::FBXmporter() {}
FBXmporter::~FBXmporter() {
	for (int a = 0; a < bufferArray.size(); a++) {
		delete[] bufferArray[a]->buffer;
		delete bufferArray[a];
	}
	bufferArray.clear();
}
std::vector<theBuffer*>* FBXmporter::saveToOurFile(const char* originPath, const char* destinationPath) {
	//////////////////////////////////////////// read the fbx file and save de data in buffers ////////////////////////////////////////////
	readFromFBX(originPath);


	//////////////////////////////////////////// write the buffers data in our custom file format ////////////////////////////////////////////	
	std::string path = destinationPath;
	LCG uidGenerator;
	std::string i;
	int UID = 0;
	for (int a = 0; a < 11; a++) {
		
		i += std::to_string(uidGenerator.IntFast());
		if (i.size() > 8) {
			int v = i.size()-8;
			i=i.substr(0, 8);
			break;
		}
		else if (i.size() == 8) {
			break;
		}
	}
	for (int a = 0; a < bufferArray.size(); a++) {
		path = path + i;//generate UID
		path = path + std::to_string(a);
		path = path + std::to_string(bufferArray.size());
		path = path + ".msh";
		saveInFile(path.c_str(), bufferArray[a]->buffer, bufferArray[a]->size);
		path.clear();
		path = destinationPath;
	}
	return &bufferArray;
}
void  FBXmporter::readFromFBX(const char* originPath) {
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(originPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_SplitLargeMeshes | aiProcess_OptimizeMeshes);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}

	
	ProcessNode(scene->mRootNode, scene);

	
}
std::vector<theBuffer*>* FBXmporter::loadFromOurFile(const char* originPath, const char* UID, const char* mesh, const char* Numbermesh, const char* extension)
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
		App->editor->AddLog("Error openenig the file %s", first.c_str());
		return NULL;
	}
	int size = getFileSize(first.c_str(), file);
	if (size == -1) {
		App->editor->AddLog("the file cannot be opened for reading");
		return NULL;
	}
	char* bufferData = new char[size];

	PHYSFS_read(file, bufferData, size, 1);
	int numMeshes = std::stoi(NumMesh);
	bufferArray.push_back(procesOneOfOurFile(bufferData, size));
	PHYSFS_close(file);
	delete[] bufferData;
	bufferData = nullptr;
	for (int a = 1; a < numMeshes; a++) {
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
		if (file == nullptr) {
			App->editor->AddLog("Error openenig the file %s", first.c_str());
			return NULL;
		}
		int size = getFileSize(first.c_str(), file);
		if (size == -1) {
			App->editor->AddLog("the file cannot be opened for reading");
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
theBuffer* FBXmporter::procesOneOfOurFile(char* data,int size) {
	
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
void FBXmporter::ProcessNode(aiNode* node, const aiScene* scene)
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

	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->rot.w = rotation.w;
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
theBuffer* FBXmporter::ProcessMesh(aiMesh* mesh, const aiScene* scene,int numOfMesh)
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
bool FBXmporter::saveMesh(const char* destinationPath, Mesh* object) 
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
char* FBXmporter::assingToBufferFBX(char* buffer, void* data, int* arrayPos, int dataSize)
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
bool FBXmporter::saveInFile(const char* path, void* data, int size) {
	
		//PHYSFS_setWriteDir(prefPath.c_str());
	
	PHYSFS_File* fileToWrite = PHYSFS_openWrite(path);
	if (fileToWrite == nullptr)
	{	
		App->editor->AddLog("Error writing in %s -> %s", path, PHYSFS_getLastError());		
		//PHYSFS_setWriteDir(".");	
		return false;
	}
	App->editor->AddLog("Mesh saved in file %s",path);
	PHYSFS_write(fileToWrite, data, size,1);
	PHYSFS_close(fileToWrite);

	return true;
}

int  FBXmporter::getFileSize(const char* path, PHYSFS_File* file) {
	
	if (file == nullptr) {
		App->editor->AddLog("Error reading in %s -> %s", path, PHYSFS_getLastError());
		return -1;
	}
	return PHYSFS_fileLength(file);
}
Mesh* FBXmporter::readFile(const char* path) {
	
	PHYSFS_File* file = PHYSFS_openRead(path);
	if (file == nullptr) {
		App->editor->AddLog("Error openenig the file %s", path);
		return NULL;
	}
	int size = getFileSize(path, file);
	if (size == -1) {
		App->editor->AddLog("the file cannot be opened for reading");
		return NULL;
	}
	char* bufferData = new char[size];
	PHYSFS_read(file, bufferData, size, 1);
	PHYSFS_close(file);		
	return getNewMeshFBX(bufferData);
}
Mesh* FBXmporter::getNewMeshFBX(char* data) {
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
