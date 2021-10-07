#pragma once
#include "glmath.h"
#include "Color.h"

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