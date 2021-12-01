#pragma once
#include "glmath.h"
#include "Color.h"
#include <vector>
#include"MathGeoLib.h"
#include "GL/glew.h"

#include "DevIL/include/il.h"
#include "DevIL/include/ilu.h"
#include "DevIL/include/ilut.h"

#define CHECKERS_WIDTH 64
#define CHECKERS_HEIGHT 64

typedef unsigned int GLuint;
typedef unsigned short GLushort;
typedef float GLfloat;
typedef unsigned char GLubyte;

enum PrimitivesTypes
{
	PRIMITIVE_BOUNDINGBOX,
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
	int TransformMatrixAABB();
	bool found;
	bool wireframe = false;

	float* matrix = nullptr;

	virtual std::vector<float3> getVertex();

protected:
	PrimitivesTypes type;

public:
	unsigned int VBO;
	unsigned int EBO;
	unsigned int TBO;
	GLuint textureId;
	GLuint aTextureId;
	AABB aabb;
	Color color;
	mat4x4 transform;
	bool axis, wire;
};

class BoundingBoxes : public Primitives
{
public:
	BoundingBoxes(float3 pos, float3 sca, float3 maxPoint, float3 minPoint);
	~BoundingBoxes();
	void DrawCube();

private:
	std::vector<float3> vertices;
	std::vector<uint> indices;
};

class MyCube : public Primitives
{
public:
	MyCube(float3 pos, float3 sca);
	~MyCube();
	void DrawCube();
public:
	GLuint imageID;
	std::vector<float3> getVertex()override;
	std::vector<float3> vertices;
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
	GLuint imageID;
	std::vector<float3> getVertex()override;

public:
	std::vector<float3> vertices;
	std::vector<uint> indices;
	std::vector<float> texCoords;
};

class MyCylinder : public Primitives
{
public:
	MyCylinder(float3 pos, float3 sca);
	MyCylinder(float3 pos, float3 sca, float baseRadius, float topRadius, float height, int sectors, int sectorCounts, int stacks, bool smooth);
	~MyCylinder();
	std::vector<float3> GetUnitCircleVertices();
	void BuildVerticalSmooth();
	void DrawCylinder();
	void Initialize();
public:
	GLuint imageID;
	std::vector<float3> getVertex()override;

private:
	std::vector<float> unitCircleVertices;
	std::vector<float> normals;
	std::vector<unsigned int> lineIndices;

	float baseRadius;
	float topRadius;
	float height;
	int sectors;
	int stacks;
	bool smooth;
	int sectorCount;
	float radius;

public:
	std::vector<float3> vertices;
	std::vector<unsigned int> indices;
	std::vector<float> texCoords;
};

class MySphere : public Primitives
{
public:
	GLuint imageID;
	std::vector<float3> getVertex()override;

public:
	MySphere(float radius, int sectorCount, int stackCount, float3 pos, float3 sca);
	~MySphere();
	void DrawSphere();
private:
	uint bufferIndices;
	uint bufferVertex;

public:
	std::vector<GLushort> indices;
	std::vector<float3> normals;
	std::vector<GLfloat> texCoords;
	std::vector<float3> vertices;
};

class Grid : public Primitives
{
public:
	Grid();
	Grid(float x, float y, float z, float d);
	void InnerRender() const;
	void DrawGrid() const;

public:
	GLuint imageID;

public:
	Vec3 normal;
	float constant;
};


class MyPlane : public Primitives
{
public:
	MyPlane(float3 pos, float3 sca);
	~MyPlane();
	void DrawPlane();
	std::vector<float3> getVertex()override;

public:
	GLuint imageID;

public:
	std::vector<GLushort> indices;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> texCoords;
	std::vector<float3> vertices;
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