#pragma once
#include "Globals.h"
#include <string>

class FileDialog
{
public:
	static std::string LoadScene(const char* filter);
	static std::string SaveScene(const char* filter);
};