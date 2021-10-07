#pragma once
#include "glmath.h"
#include "Color.h"
#include <vector>

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
	MyCube();
	void DrawCube();
private:
	uint num_vertices;
	uint num_indices;
	uint my_indices;
};

class MyPyramid : public Primitives
{
public:
	MyPyramid();
	void DrawPyramid();
private:
	uint num_pyramid_vertices;
	uint num_pyramid_indices;
	uint my_pyramid_indices;
};

class MyCylinder : public Primitives
{
public:
	MyCylinder();
	MyCylinder(float baseRadius, float topRadius, float height, int sectors, int stacks, bool smooth);
	~MyCylinder();
	std::vector<float> GetUnitCircleVertices();
	void BuildVerticalSmooth();
	void Draw();
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

	uint buffer;
};
//// ============================================
//class MySphere : public Primitives
//{
//public:
//	MySphere();
//};
//
//// ============================================
//class MyCylinder : public Primitives
//{
//public:
//	MyCylinder();
//};