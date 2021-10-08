#pragma once
#include "glmath.h"
#include "Color.h"
#include <vector>

typedef unsigned int GLuint;
typedef unsigned short GLushort;
typedef float GLfloat;

enum PrimitivesTypes
{
	PRIMITIVE_MYCUBE,
	PRIMITIVE_MYSPHERE,
	PRIMITIVE_MYCYLINDER,
	PRIMITIVE_MYPYRAMID,
};

class Primitives
{
public:

	Primitives();
	PrimitivesTypes	GetType() const;

protected:
	PrimitivesTypes type;
};



// ============================================
class MyCube : public Primitives
{
public:
	MyCube(float x, float y, float z, float X, float Y, float Z);
	~MyCube();
	void DrawCube();
private:
	uint VAO;
	uint bufferIndices;
	uint bufferVertex;

	std::vector<float> vertices;
	std::vector<uint> indices;
};

class MyPyramid : public Primitives
{
public:
	MyPyramid(float x, float y, float z, float X, float Y, float Z);
	~MyPyramid();
	void DrawPyramid();
private:
	uint VAO;
	uint bufferIndices;
	uint bufferVertex;

	std::vector<float> vertices;
	std::vector<uint> indices;
};

class MyCylinder : public Primitives
{
public:
	MyCylinder();
	MyCylinder(float baseRadius, float topRadius, float height, int sectors, int sectorCounts, int stacks, bool smooth);
	~MyCylinder();
	std::vector<float> GetUnitCircleVertices();
	void BuildVerticalSmooth();
	void DrawCylinder();
	void Initialize();

private:
	std::vector<float> unitCircleVertices;
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texCoords;
	std::vector<unsigned int> indices;
	std::vector<unsigned int> lineIndices;

	float baseRadius;
	float topRadius;
	float height;
	int sectors;
	int stacks;
	bool smooth;
	int sectorCount;
	float radius;

	uint VAO;
	uint bufferIndices;
	uint bufferVertex;
};

class MySphere : public Primitives
{
protected:
	std::vector<GLushort> indices;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> texCoords;
	std::vector<GLfloat> vertices;
public:
	MySphere(float radius, uint rings, uint sectors);
	~MySphere();
	void DrawSphere();
private:
	uint VAO;
	uint bufferIndices;
	uint bufferVertex;
};
