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

	virtual void	Render() const;
	virtual void	InnerRender() const;
	void			SetPos(float x, float y, float z);
	void			SetRotation(float angle, const Vec3& u);
	void			Scale(float x, float y, float z);
	PrimitivesTypes	GetType() const;

public:

	Color color;
	mat4x4 transform;
	bool axis, wire;

protected:
	PrimitivesTypes type;
};



// ============================================
class MyCube : public Primitives
{
public:
	MyCube();
};



//// ============================================
//class MySphere : public Primitives
//{
//public:
//	MySphere();
//	MySphere(float radius);
//	void InnerRender() const;
//public:
//	float radius;
//};
//
//
//
//// ============================================
//class MyCylinder : public Primitives
//{
//public:
//	MyCylinder();
//	MyCylinder(float radius, float height);
//	void InnerRender() const;
//public:
//	float radius;
//	float height;
//};
//
//
//
//// ============================================
//class MyPyramid : public Primitives
//{
//public:
//	MyPyramid();
//public:
//
//};