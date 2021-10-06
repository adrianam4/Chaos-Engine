#include "Globals.h"
#include "GL/glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Primitives.h"


// ------------------------------------------------------------
PrimitivesTypes Primitives::GetType() const
{
	return type;
}

// ------------------------------------------------------------
void Primitives::Render() const
{
	glPushMatrix();
	glMultMatrixf(transform.M);

	if (axis == true)
	{
		// Draw Axis Grid
		glLineWidth(2.0f);

		glBegin(GL_LINES);

		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
		glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);

		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);

		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
		glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
		glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);

		glEnd();

		glLineWidth(1.0f);
	}

	glColor3f(color.r, color.g, color.b);

	if (wire)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	InnerRender();

	glPopMatrix();
}

// ------------------------------------------------------------
void Primitives::InnerRender() const
{
	glPointSize(5.0f);

	glBegin(GL_POINTS);

	glVertex3f(0.0f, 0.0f, 0.0f);

	glEnd();

	glPointSize(1.0f);
}

// ------------------------------------------------------------
void Primitives::SetPos(float x, float y, float z)
{
	transform.translate(x, y, z);
}

// ------------------------------------------------------------
void Primitives::SetRotation(float angle, const Vec3& u)
{
	transform.rotate(angle, u);
}

// ------------------------------------------------------------
void Primitives::Scale(float x, float y, float z)
{
	transform.scale(x, y, z);
}

//// CUBE ============================================
//MyCube::MyCube() : Primitives()
//{
//
//	type = PrimitivesTypes::PRIMITIVE_MYCUBE;
//
//	static GLfloat vertices[] =
//	{
//		-1, 0, 0,
//		1, 0, 0,
//		1, 2, 0,
//		-1, 2, 0,
//
//		-1, 0, -2,
//		1, 0, -2,
//		1, 2, -2,
//		-1, 2, -2
//	};
//
//	static GLuint indices[] =
//	{
//		0,1,2, 0,2,3,
//		6,5,4, 7,6,4,
//		1,5,6, 1,6,2,
//		4,0,3, 4,3,7,
//		3,2,6, 3,6,7,
//		5,1,0, 4,5,0
//	};
//
//	static uint my_indices = 0;
//	static uint num_indices = 36;
//	glGenBuffers(1, (GLuint*)&(my_indices));
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * num_indices, indices, GL_STATIC_DRAW);
//	glVertexPointer(3, GL_FLOAT, 0, vertices);
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
//	glEnableClientState(GL_VERTEX_ARRAY);
//	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);
//	glDisableClientState(GL_VERTEX_ARRAY);
//}


//// PYRAMID ============================================
//MyPyramid::MyPyramid() : Primitives()
//{
//	type = PrimitivesTypes::PRIMITIVE_MYPYRAMID;
//}
// 
// 
// 
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