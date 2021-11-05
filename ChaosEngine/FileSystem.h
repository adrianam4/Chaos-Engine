#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include "Module.h"
#include <vector>
#include <string>

struct SDL_RWops;
int close_sdl_rwops(SDL_RWops *rw);

struct aiFileIO;
struct BASS_FILEPROCS;

class FileSystem : public Module
{
public:

	FileSystem(Application* app, bool startEnabled = true);

	// Destructor
	~FileSystem();

	// Utility functions
	bool Init();
	bool AddPath(const char* pathOrZip);
	bool Exists(const char* file) const;
	bool IsDirectory(const char* file) const;
    void CreateDirectory(const char* directory);
	void DiscoverFiles(const char* directory, std::vector<std::string>& fileList, std::vector<std::string>& dirList) const;
	bool CopyFromOutsideFS(const char* fullPath, const char* destination);
	bool Copy(const char* source, const char* destination);
	void SplitFilePath(const char* fullPath, std::string* path, std::string* file = nullptr, std::string* extension = nullptr) const;
	void NormalizePath(char* fullPath) const;
	void NormalizePath(std::string& fullPath) const;
	unsigned GetFileSize(const char* path) const;

	// Open for Read/Write
	unsigned int Load(const char* path, const char* file, char** buffer) const;
	unsigned int Load(const char* file, char** buffer) const;
	SDL_RWops* Load(const char* file) const;

	// IO interfaces for other libs to handle files via PHYSfs
	aiFileIO* GetAssimpIO();

	unsigned int Save(const char* file, const void* buffer, unsigned int size, bool append = false) const;
	bool SaveUnique(std::string& output, const void* buffer, uint size, const char* path, const char* prefix, const char* extension);
	bool Remove(const char* file);
	bool CreateDir(const char* directory);

	const char* GetBasePath() const;
	const char* GetWritePath() const;
	const char* GetReadPaths() const;

private:

	void CreateAssimpIO();

private:

	aiFileIO* AssimpIO = nullptr;
	char* basePath;
};

#endif // __FILESYSTEM_H__