#include "MaterialImporter.h"

MaterialImporter::MaterialImporter()
{
}

MaterialImporter::~MaterialImporter()
{
}

bool MaterialImporter::SaveMaterial(const char* destinationPath, const char* sourcePath)
{
	return false;
}

const char* MaterialImporter::ReadFile(const char* path)
{
	return nullptr;
}

bool MaterialImporter::SaveInFile(const char* path, void* data, int size)
{
	return false;
}

char* MaterialImporter::AssingToBuffer(char* buffer, void* data, int* arrayPos, int dataSize)
{
	return nullptr;
}

int MaterialImporter::GetFileSize(const char* path, PHYSFS_File* file)
{
	return 0;
}

const char* MaterialImporter::GetNewMaterial(char* data)
{
	return nullptr;
}
