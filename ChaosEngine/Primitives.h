#pragma once
#include "glmath.h"
#include "Color.h"
#include <vector>

#include "GL/glew.h"

#include "DevIL/include/IL/il.h"
#include "DevIL/include/IL/ilu.h"
#include "DevIL/include/IL/ilut.h"

#define CHECKERS_WIDTH 64
#define CHECKERS_HEIGHT 64

typedef unsigned int GLuint;
typedef unsigned short GLushort;
typedef float GLfloat;
typedef unsigned char GLubyte;

enum PrimitivesTypes
{
	PRIMITIVE_MYCUBE,
	PRIMITIVE_MYPLANE3D,
	PRIMITIVE_MYSPHERE,
	PRIMITIVE_MYCYLINDER,
	PRIMITIVE_MYPYRAMID,
	PRIMITIVE_MYPLANE,
	PRIMITIVE_MYPOINT,
	PRIMITIVE_MYLINE,
};

class Primitives
{
public:

	Primitives();
	PrimitivesTypes	GetType() const;
	float3 position;
	float3 scale;
	uint id;
	int TransformMatrix();
	bool found;
protected:
	PrimitivesTypes type;
public:
	unsigned int VBO;
	unsigned int EBO;
	unsigned int TBO;
	GLuint textureId;
	GLuint aTextureId;

	Color color;
	mat4x4 transform;
	bool axis, wire;
};

class MyCube : public Primitives
{
public:
	MyCube(float3 pos, float3 sca);
	~MyCube();
	void DrawCube();

public:
	ILuint imageID;

private:
	std::vector<float> vertices;
	std::vector<uint> indices;
	std::vector<float> texCoords;
};

class MyPyramid : public Primitives
{
public:
	MyPyramid(float3 pos, float3 sca);
	~MyPyramid();
	void DrawPyramid();

public:
	ILuint imageID;

private:

	std::vector<float> vertices;
	std::vector<uint> indices;
	std::vector<float> texCoords;
};

class MyCylinder : public Primitives
{
public:
	MyCylinder(float3 pos);
	MyCylinder(float3 pos, float3 sca, float baseRadius, float topRadius, float height, int sectors, int sectorCounts, int stacks, bool smooth);
	~MyCylinder();
	std::vector<float> GetUnitCircleVertices();
	void BuildVerticalSmooth();
	void DrawCylinder();
	void Initialize();

public:
	ILuint imageID;

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
};

class MySphere : public Primitives
{
protected:
	std::vector<GLushort> indices;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> texCoords;
	std::vector<GLfloat> vertices;

public:
	ILuint imageID;

public:
	MySphere(float radius, int sectorCount, int stackCount, float3 pos, float3 sca);
	~MySphere();
	void DrawSphere();
private:
	uint bufferIndices;
	uint bufferVertex;
};

class MyPlane : public Primitives
{
public:
	MyPlane();
	MyPlane(float x, float y, float z, float d);
	void InnerRender() const;
	void DrawPlane() const;

public:
	ILuint imageID;

public:
	Vec3 normal;
	float constant;
};


class MyPlane3D : public Primitives
{
public:
	MyPlane3D(float3 pos, float3 sca);

	~MyPlane3D();
	void DrawPlane();

public:
	ILuint imageID;

public:
	std::vector<GLushort> indices;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> texCoords;
	std::vector<GLfloat> vertices;
};


class MyLine : public Primitives
{
public:
	MyLine();
	MyLine(float x, float y, float z);
	void DrawLine() const;
public:
	Vec3 origin;
	Vec3 destination;
};