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
};

class MyPyramid : public Primitives
{
public:
	MyPyramid();
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