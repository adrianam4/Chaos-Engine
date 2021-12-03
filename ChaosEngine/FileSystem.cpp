#include "Globals.h"
#include "Application.h"
#include "FileSystem.h"
#include "PhysFS/include/physfs.h"
#include <assimp/cfileio.h>
#include <assimp/types.h>

#pragma comment( lib, "PhysFS/libx86/physfs.lib" )
#include "mmgr.h"

using namespace std;

FileSystem::FileSystem(Application* app, bool startEnabled) : Module(app, startEnabled)
{
	// needs to be created before Init so other modules can use it
	basePath = SDL_GetBasePath();
	PHYSFS_init(nullptr);
	
	// workaround VS string directory mess
	AddPath(".");
	AddPath("./Library");

	// Generate IO interfaces
	CreateAssimpIO();
}

// Destructor
FileSystem::~FileSystem()
{
	//DeleteLibraryFiles();
	RELEASE(AssimpIO);
	PHYSFS_deinit();
}

// Called before render is available
bool FileSystem::Init()
{
	App->editor->AddLog("Loading File System\n");
	bool ret = true;

	//// Ask SDL for a write dir
	//char* write_path = SDL_GetPrefPath("Adrian Aroca and David Lira", "Chaos Engine");
	//PHYSFS_setWriteDir("Assets/Library/Textures");
	//CreateDirectory("Assets/Library/Textures");

	//SDL_free(write_path);
	if (PHYSFS_setWriteDir(".") == 0) { App->editor->AddLog("File System error while creating write dir: %s\n", PHYSFS_getLastError()); }
	if (PHYSFS_init(nullptr) == 0) {

		App->editor->AddLog("PhysFS succesfully loaded | Libs initialized\n");

	}

	CreateDir("Assets/Textures/");
	CreateDir("Assets/Models/");
	CreateDir("Library/Textures/");
	CreateDir("Library/Models/");

	return ret;
}

// Add a new zip file or folder
bool FileSystem::AddPath(const char* path_or_zip)
{
	bool ret = false;

	if (PHYSFS_mount(path_or_zip, nullptr, 1) == 0)
	{
		App->editor->AddLog("File System error while adding a path or zip: %s\n", PHYSFS_getLastError());
	}
	else
		ret = true;

	return ret;
}

// Check if a file exists
bool FileSystem::Exists(const char* file) const
{
	return PHYSFS_exists(file) != 0;
}

// Check if a file is a directory
bool FileSystem::IsDirectory(const char* file) const
{
	return PHYSFS_isDirectory(file) != 0;
}

void FileSystem::CreateDirectory(const char* directory)
{
    PHYSFS_mkdir(directory);
}

void FileSystem::DiscoverFiles(const char* directory, vector<string> & file_list, vector<string> & dir_list) const
{
	char **rc = PHYSFS_enumerateFiles(directory);
	char **i;

	string dir(directory);

	for (i = rc; *i != nullptr; i++)
	{
		if(PHYSFS_isDirectory((dir+*i).c_str()))
			dir_list.push_back(*i);
		else
			file_list.push_back(*i);
	}

	PHYSFS_freeList(rc);
}

bool FileSystem::CopyFromOutsideFS(const char * full_path, const char * destination)
{
	// Only place we acces non virtual filesystem
 	bool ret = false;

    char buf[8192];
    size_t size;

	FILE* source = nullptr;
	fopen_s(&source,full_path, "rb");
	PHYSFS_file* dest = PHYSFS_openWrite(destination);

	if (source && dest)
	{
		while (size = fread_s(buf, 8192, 1, 8192, source))
			PHYSFS_write(dest, buf, 1, size);

		fclose(source);
		PHYSFS_close(dest);
		ret = true;

		App->editor->AddLog("File System copied file [%s] to [%s]\n", full_path, destination);
	}
	else
		App->editor->AddLog("File System error while copy from [%s] to [%s]\n", full_path, destination);

	return ret;
}

bool FileSystem::Copy(const char * source, const char * destination)
{
 	bool ret = false;

    char buf[8192];

	PHYSFS_file* src = PHYSFS_openRead(source);
	PHYSFS_file* dst = PHYSFS_openWrite(destination);

	PHYSFS_sint32 size;
	if (src && dst)
	{
		while (size = (PHYSFS_sint32) PHYSFS_read(src, buf, 1, 8192))
			PHYSFS_write(dst, buf, 1, size);

		PHYSFS_close(src);
		PHYSFS_close(dst);
		ret = true;

		App->editor->AddLog("File System copied file [%s] to [%s]\n", source, destination);
	}
	else
		App->editor->AddLog("File System error while copy from [%s] to [%s]\n", source, destination);

	return ret;
}

void FileSystem::SplitFilePath(const char * full_path, std::string * texturePath, std::string * file, std::string * extension) const
{
	if (full_path != nullptr)
	{
		string full(full_path);
		NormalizePath(full);
		size_t pos_separator = full.find_last_of("\\/");
		size_t pos_dot = full.find_last_of(".");

		if (texturePath != nullptr)
		{
			if (pos_separator < full.length())
				*texturePath = full.substr(0, pos_separator + 1);
			else
				texturePath->clear();
		}

		if (file != nullptr)
		{
			if (pos_separator < full.length())
				*file = full.substr(pos_separator + 1);
			else
				*file = full;
		}

		if (extension != nullptr)
		{
			if (pos_dot < full.length())
				*extension = full.substr(pos_dot + 1);
			else
				extension->clear();
		}
	}
}

// Flatten filenames to always use lowercase and / as folder separator
char normalize_char(char c)
{
	if (c == '\\')
		return '/';
	return tolower(c);
}

void FileSystem::NormalizePath(char * full_path) const
{
	int len = strlen(full_path);
	for (int i = 0; i < len; ++i)
	{
		if (full_path[i] == '\\')
			full_path[i] = '/';
		else
			full_path[i] = tolower(full_path[i]);
	}
}

void FileSystem::NormalizePath(std::string & full_path) const
{
	for (string::iterator it = full_path.begin(); it != full_path.end(); ++it)
	{
		if (*it == '\\')
			*it = '/';
		else
			*it = tolower(*it);
	}
}

void FileSystem::DeleteLibraryFiles()
{
	std::vector<std::string> files;
	std::vector<std::string> directories;
	DiscoverFiles("Library/", files, directories);
	for (uint i = 0; i < directories.size(); i++)
	{
		std::string toDelete = "Library/" + directories[i] + "/";
		std::vector<std::string> files2;
		std::vector<std::string> directories2;
		DiscoverFiles(toDelete.c_str(), files2, directories2);
		for (uint j = 0; j < files2.size(); j++)
		{
			std::string toDelete2 = toDelete + files2[j];
			Remove(toDelete2.c_str());
		}
	}
}

unsigned FileSystem::GetFileSize(const char* texturePath) const
{
	PHYSFS_File* file = PHYSFS_openRead(texturePath);

	if (file == nullptr)
		return 0;

	return PHYSFS_fileLength(file);
}

unsigned int FileSystem::Load(const char * texturePath, const char * file, char ** buffer) const
{
	string full_path(texturePath);
	full_path += file;
	return Load(full_path.c_str(), buffer);
}

// Read a whole file and put it in a new buffer
uint FileSystem::Load(const char* file, char** buffer) const
{
	uint ret = 0;

	PHYSFS_file* fs_file = PHYSFS_openRead(file);

	if(fs_file != nullptr)
	{
		PHYSFS_sint32 size = (PHYSFS_sint32) PHYSFS_fileLength(fs_file);

		if(size > 0)
		{
			*buffer = new char[size];
			uint readed = (uint) PHYSFS_read(fs_file, *buffer, 1, size);
			if(readed != size)
			{
				App->editor->AddLog("File System error while reading from file %s: %s\n", file, PHYSFS_getLastError());
				RELEASE(buffer);
			}
			else
				ret = readed;
		}

		if(PHYSFS_close(fs_file) == 0)
			App->editor->AddLog("File System error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
		App->editor->AddLog("File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}

// Read a whole file and put it in a new buffer
SDL_RWops* FileSystem::Load(const char* file) const
{
	char* buffer;
	int size = Load(file, &buffer);

	if(size > 0)
	{
		SDL_RWops* r = SDL_RWFromConstMem(buffer, size);
		if(r != nullptr)
			r->close = close_sdl_rwops;

		return r;
	}
	else
		return nullptr;
}

int close_sdl_rwops(SDL_RWops *rw)
{
	RELEASE(rw->hidden.mem.base);
	SDL_FreeRW(rw);
	return 0;
}

// Save a whole buffer to disk
uint FileSystem::Save(const char* file, const void* buffer, unsigned int size, bool append) const
{
	unsigned int ret = 0;

	bool overwrite = PHYSFS_exists(file) != 0;
	PHYSFS_file* fs_file = (append) ? PHYSFS_openAppend(file) : PHYSFS_openWrite(file);

	if(fs_file != nullptr)
	{
		uint written = (uint) PHYSFS_write(fs_file, (const void*)buffer, 1, size);
		if (written != size)
		{
			App->editor->AddLog("File System error while writing to file %s: %s\n", file, PHYSFS_getLastError());
		}
		else
		{
			if (append == true)
				App->editor->AddLog("Added %u data to [%s%s]\n", size, PHYSFS_getWriteDir(), file);
			else if(overwrite == true)
				App->editor->AddLog("File [%s%s] overwritten with %u bytes\n", PHYSFS_getWriteDir(), file, size);
			else if(overwrite == false)
				App->editor->AddLog("New file created [%s%s] of %u bytes\n", PHYSFS_getWriteDir(), file, size);

			ret = written;
		}

		if(PHYSFS_close(fs_file) == 0)
			App->editor->AddLog("File System error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
		App->editor->AddLog("File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}

bool FileSystem::SaveUnique(string& name, const void * buffer, uint size, const char * texturePath, const char * prefix, const char * extension)
{
	//char result[250];
	////MODULE RESOURCES
	//sprintf_s(result, 250, "%s%s_%llu.%s", path, prefix, App->resources->GenerateNewUID(), extension);
	//NormalizePath(result);
	//if (Save(result, buffer, size) > 0)
	//{
	//	name = result;
	//	return true;
	//}
	return false;
}

bool FileSystem::Remove(const char * file)
{
	bool ret = false;

	if (file != nullptr)
	{
		if (PHYSFS_delete(file) == 0)
		{
			App->editor->AddLog("File deleted: [%s]\n", file);
			ret = true;
		}
		else
			App->editor->AddLog("File System error while trying to delete [%s]: \n", file, PHYSFS_getLastError());
	}

	return ret;
}

bool FileSystem::CreateDir(const char* directory)
{
	if (!IsDirectory(directory))
	{
		PHYSFS_mkdir(directory);
		AddPath(directory);
		return true;
	}
	return false;
}

const char * FileSystem::GetBasePath() const
{
	return PHYSFS_getBaseDir();
}

const char * FileSystem::GetWritePath() const
{
	return PHYSFS_getWriteDir();
}

const char * FileSystem::GetReadPaths() const
{
	static char paths[512];

	paths[0] = '\0';

	char **texturePath;
	for (texturePath = PHYSFS_getSearchPath(); *texturePath != nullptr; texturePath++)
	{
		strcat_s(paths, 512, *texturePath);
		strcat_s(paths, 512, "\n");
	}

	return paths;
}

// -----------------------------------------------------
// ASSIMP IO
// -----------------------------------------------------

size_t AssimpWrite(aiFile* file, const char* data, size_t size, size_t chunks)
{
	PHYSFS_sint64 ret = PHYSFS_write((PHYSFS_File*)file->UserData, (void*)data, size, chunks);
	if(ret == -1)
		App->editor->AddLog("File System error while WRITE via assimp: %s\n", PHYSFS_getLastError());

	return (size_t) ret;
}

size_t AssimpRead(aiFile* file, char* data, size_t size, size_t chunks)
{
	PHYSFS_sint64 ret = PHYSFS_read((PHYSFS_File*)file->UserData, (void*)data, size, chunks);
	if(ret == -1)
		App->editor->AddLog("File System error while READ via assimp: %s\n", PHYSFS_getLastError());

	return (size_t) ret;
}

size_t AssimpTell(aiFile* file)
{
	PHYSFS_sint64 ret = PHYSFS_tell((PHYSFS_File*)file->UserData);
	if(ret == -1)
		App->editor->AddLog("File System error while TELL via assimp: %s\n", PHYSFS_getLastError());

	return (size_t) ret;
}

size_t AssimpSize(aiFile* file)
{
	PHYSFS_sint64 ret = PHYSFS_fileLength((PHYSFS_File*)file->UserData);
	if(ret == -1)
		App->editor->AddLog("File System error while SIZE via assimp: %s\n", PHYSFS_getLastError());

	return (size_t) ret;
}

void AssimpFlush(aiFile* file)
{
	if(PHYSFS_flush((PHYSFS_File*)file->UserData) == 0)
		App->editor->AddLog("File System error while FLUSH via assimp: %s\n", PHYSFS_getLastError());
}

aiReturn AssimpSeek(aiFile* file, size_t pos, aiOrigin from)
{
	int res = 0;

	switch (from)
	{
	case aiOrigin_SET:
		res = PHYSFS_seek((PHYSFS_File*)file->UserData, pos);
		break;
	case aiOrigin_CUR:
		res = PHYSFS_seek((PHYSFS_File*)file->UserData, PHYSFS_tell((PHYSFS_File*)file->UserData) + pos);
		break;
	case aiOrigin_END:
		res = PHYSFS_seek((PHYSFS_File*)file->UserData, PHYSFS_fileLength((PHYSFS_File*)file->UserData) + pos);
		break;
	}

	if(res == 0)
		App->editor->AddLog("File System error while SEEK via assimp: %s\n", PHYSFS_getLastError());

	return (res != 0) ? aiReturn_SUCCESS : aiReturn_FAILURE;
}

aiFile* AssimpOpen(aiFileIO* io, const char* name, const char* format)
{
	static aiFile file;

	file.UserData = (char*) PHYSFS_openRead(name);
	file.ReadProc = AssimpRead;
	file.WriteProc = AssimpWrite;
	file.TellProc = AssimpTell;
	file.FileSizeProc = AssimpSize;
	file.FlushProc= AssimpFlush;
	file.SeekProc = AssimpSeek;

	return &file;
}

void AssimpClose(aiFileIO* io, aiFile* file)
{
	if (PHYSFS_close((PHYSFS_File*)file->UserData) == 0)
		App->editor->AddLog("File System error while CLOSE via assimp: %s\n", PHYSFS_getLastError());
}

void FileSystem::CreateAssimpIO()
{
	RELEASE(AssimpIO);

	AssimpIO = new aiFileIO;
	AssimpIO->OpenProc = AssimpOpen;
	AssimpIO->CloseProc = AssimpClose;
}

aiFileIO * FileSystem::GetAssimpIO()
{
	return AssimpIO;
}