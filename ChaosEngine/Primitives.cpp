#include "Globals.h"
#include "GL/glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Primitives.h"

// ------------------------------------------------------------
Primitives::Primitives()
{}

// ------------------------------------------------------------
PrimitivesTypes Primitives::GetType() const
{
	return type;
}

//// CUBE ============================================
MyCube::MyCube() : Primitives()
{

	type = PrimitivesTypes::PRIMITIVE_MYCUBE;

	static GLfloat vertices[] =
	{
		-1, 0, 0,
		1, 0, 0,
		1, 2, 0,
		-1, 2, 0,

		-1, 0, -2,
		1, 0, -2,
		1, 2, -2,
		-1, 2, -2
	};

	static GLuint indices[] =
	{
		0,1,2, 0,2,3,
		6,5,4, 7,6,4,
		1,5,6, 1,6,2,
		4,0,3, 4,3,7,
		3,2,6, 3,6,7,
		5,1,0, 4,5,0
	};

	my_indices = 0;
	num_indices = 36;
	glGenBuffers(1, (GLuint*)&(my_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * num_indices, indices, GL_STATIC_DRAW);
	glVertexPointer(3, GL_FLOAT, 0, vertices);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);

}

void MyCube::DrawCube()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
}


//// PYRAMID ============================================
MyPyramid::MyPyramid() : Primitives()
{
	type = PrimitivesTypes::PRIMITIVE_MYPYRAMID;

	static GLfloat pyramid_vertices[] =
	{
		3, 0, 2,
		4, 0, 3,
		3, 0, 4,
		2, 0, 3,

		3, 2, 3
	};

	static GLuint pyramid_indices[] =
	{
		4,1,0, 4,2,1,
		2,4,3, 3,4,0,
		0,1,3, 1,2,3
	};

	my_pyramid_indices = 0;
	num_pyramid_indices = 18;
	glGenBuffers(1, (GLuint*)&(my_pyramid_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_pyramid_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * num_pyramid_indices, pyramid_indices, GL_STATIC_DRAW);
	glVertexPointer(3, GL_FLOAT, 0, pyramid_vertices);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_pyramid_indices);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawElements(GL_TRIANGLES, num_pyramid_indices, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
}
 
void MyPyramid::DrawPyramid() 
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_pyramid_indices);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawElements(GL_TRIANGLES, num_pyramid_indices, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
}
 
//// SPHERE ============================================
//MySphere::MySphere() : Primitives()
//{
//	type = PrimitivesTypes::PRIMITIVE_MYSPHERE;
//}
//
//
// 
//// CYLINDER ============================================
//MyCylinder::MyCylinder() : Primitives()
//{
//	type = PrimitivesTypes::PRIMITIVE_MYCYLINDER;
//}