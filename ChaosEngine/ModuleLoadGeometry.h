#pragma once
#include "Module.h"
#include "Globals.h"

#include "MathGeoLib/src/MathGeoLib.h"

class Mesh
{
public:
	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;

	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;
};

class LoadGeometry
{
public:
	LoadGeometry();
	~LoadGeometry();
	void LoadFile(const char* file_path);

public:
	Mesh ourMesh;
};