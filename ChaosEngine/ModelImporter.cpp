#include"Application.h"
#include"ModelImporter.h"
#include "mmgr.h"

FBXImporter::FBXImporter() {}

FBXImporter::~FBXImporter() {}

bool FBXImporter::saveMesh(const char* destinationPath, Mesh* object) 
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
	float* indexAux = new float[object->indices.size()];
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
	buffer=assingToBuffer(buffer, &numOf, &pos,1 * sizeof(int));
	
	
	numOf = object->indices.size();
	buffer = assingToBuffer(buffer, &numOf,  &pos,1 * sizeof(int));

	numOf = object->texCoords.size();
	buffer = assingToBuffer(buffer, &numOf, &pos,1 * sizeof(int));

	
	buffer = assingToBuffer(buffer, vertAux, &pos, 3 * object->vertices.size() * sizeof(float));

	buffer = assingToBuffer(buffer, indexAux, &pos, object->indices.size() * sizeof(int));

	buffer = assingToBuffer(buffer, textureAux, &pos, object->texCoords.size()*sizeof(float));

	
	delete[] vertAux;
	delete[] indexAux;
	delete[] textureAux;
	//assingToBuffer(buffer, &object->vertices[0], sizeof(Vertex)*3* object->vertices.size(), currentSize,&pos);
	saveInFile(destinationPath, buffer, pos);
	delete[] buffer;
	return true;
}
char* FBXImporter::assingToBuffer(char* buffer, void* data, int* arrayPos, int dataSize)
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
bool FBXImporter::saveInFile(const char* path, void* data, int size) {
	
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

int  FBXImporter::getFileSize(const char* path, PHYSFS_File* file) {
	
	if (file == nullptr) {
		App->editor->AddLog("Error reading in %s -> %s", path, PHYSFS_getLastError());
		return -1;
	}
	return PHYSFS_fileLength(file);
}
Mesh* FBXImporter::readFile(const char* path) {
	
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
	return getNewMesh(bufferData);
}
Mesh* FBXImporter::getNewMesh(char* data) {
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

	
	float* auxVertex = new float[numVertex * 3];

	memcpy(auxVertex, &data[pointer], 3 * sizeof(float)* numVertex);
	pointer += (3 * sizeof(float) * numVertex);


	float* auxIndex = new float[numIndex];

	memcpy(auxIndex, &data[pointer],sizeof(int) * numIndex);
	pointer += (sizeof(int) * numIndex);


	float* auxTextureCoor = new float[numTextCoord];

	memcpy(auxTextureCoor, &data[pointer], sizeof(float) * numTextCoord);
	pointer += (sizeof(float) * numTextCoord);


	std::vector<Vertex> vertices;
	int i = 0;
	while (i < numVertex*3)
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
	return NULL;
}