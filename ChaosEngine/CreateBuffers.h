#pragma once
#include "glmath.h"
#include "Color.h"
#include <vector>

typedef unsigned int uint;

class CreateBuffers 
{
public:
	CreateBuffers(std::vector<float> vertices, std::vector<uint> indices);
	CreateBuffers(float* vertices, uint* indices);
	~CreateBuffers();
public:
	uint bufferVertex;
	uint bufferIndices;
};