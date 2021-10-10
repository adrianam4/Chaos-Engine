#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "Primitives.h"

#include "GL/glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>


// ------------------------------------------------------------
Primitives::Primitives()
{}

// ------------------------------------------------------------
PrimitivesTypes Primitives::GetType() const
{
	return type;
}

//// CUBE ============================================
MyCube::MyCube(float x, float y, float z, float X, float Y, float Z) : Primitives()
{

	type = PrimitivesTypes::PRIMITIVE_MYCUBE;

	vertices.push_back((-1 + x) * X); vertices.push_back((0 + y) * Y); vertices.push_back((0 + z) * Z);
	vertices.push_back((1 + x) * X); vertices.push_back((0 + y) * Y); vertices.push_back((0 + z) * Z);
	vertices.push_back((1 + x) * X); vertices.push_back((2 + y) * Y); vertices.push_back((0 + z) * Z);
	vertices.push_back((-1 + x) * X); vertices.push_back((2 + y) * Y); vertices.push_back((0 + z) * Z);

	vertices.push_back((-1 + x) * X); vertices.push_back((0 + y) * Y); vertices.push_back((-2 + z) * Z);
	vertices.push_back((1 + x) * X); vertices.push_back((0 + y) * Y); vertices.push_back((-2 + z) * Z);
	vertices.push_back((1 + x) * X); vertices.push_back((2 + y) * Y); vertices.push_back((-2 + z) * Z);
	vertices.push_back((-1 + x) * X); vertices.push_back((2 + y) * Y); vertices.push_back((-2 + z) * Z);

	indices.push_back(0); indices.push_back(1); indices.push_back(2); indices.push_back(0); indices.push_back(2); indices.push_back(3);
	indices.push_back(6); indices.push_back(5); indices.push_back(4); indices.push_back(7); indices.push_back(6); indices.push_back(4);
	indices.push_back(1); indices.push_back(5); indices.push_back(6); indices.push_back(1); indices.push_back(6); indices.push_back(2);
	indices.push_back(4); indices.push_back(0); indices.push_back(3); indices.push_back(4); indices.push_back(3); indices.push_back(7);
	indices.push_back(3); indices.push_back(2); indices.push_back(6); indices.push_back(3); indices.push_back(6); indices.push_back(7);
	indices.push_back(5); indices.push_back(1); indices.push_back(0); indices.push_back(4); indices.push_back(5); indices.push_back(0);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &bufferVertex);
	glBindBuffer(GL_ARRAY_BUFFER, bufferVertex);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &bufferIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);

	glEnableVertexAttribArray(0);
}

MyCube::~MyCube()
{
	vertices.clear();
	indices.clear();
}

void MyCube::DrawCube()
{
	glLineWidth(5.0f);

	if (App->editor->wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}


//// PYRAMID ============================================
MyPyramid::MyPyramid(float x, float y, float z, float X, float Y, float Z) : Primitives()
{
	type = PrimitivesTypes::PRIMITIVE_MYPYRAMID;

	vertices.push_back((-1 + x) * X); vertices.push_back((0 + y) * Y); vertices.push_back((-1 + z) * Z);
	vertices.push_back((1 + x) * X);vertices.push_back((0 + y) * Y);vertices.push_back((-1 + z) * Z);
	vertices.push_back((0 + x) * X); vertices.push_back((2 + y) * Y); vertices.push_back((0 + z) * Z);
	vertices.push_back((1 + x) * X);vertices.push_back((0 + y) * Y);vertices.push_back((1 + z) * Z);
	vertices.push_back((-1 + x) * X);vertices.push_back((0 + y) * Y);vertices.push_back((1 + z) * Z);

	indices.push_back(2); indices.push_back(1); indices.push_back(0);
	indices.push_back(2); indices.push_back(3); indices.push_back(1);
	indices.push_back(4); indices.push_back(3); indices.push_back(2);
	indices.push_back(4); indices.push_back(2); indices.push_back(0);
	indices.push_back(0); indices.push_back(1); indices.push_back(3);
	indices.push_back(0); indices.push_back(3); indices.push_back(4);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &bufferVertex);
	glBindBuffer(GL_ARRAY_BUFFER, bufferVertex);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &bufferIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);

	glEnableVertexAttribArray(0);
}

MyPyramid::~MyPyramid()
{
	vertices.clear();
	indices.clear();
}
 
void MyPyramid::DrawPyramid() 
{
	if (App->editor->wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

MyCylinder::MyCylinder() : Primitives()
{
	type = PrimitivesTypes::PRIMITIVE_MYCYLINDER;

	baseRadius = 2.0f;
	topRadius = 2.0f;
	height = 5.0f;
	sectors = 16;
	sectorCount = 16;
	stacks = 5;
	radius = topRadius;

	smooth = true;

	Initialize();
}

MyCylinder::MyCylinder(float baseRadius, float topRadius, float height, int sectors, int sectorCounts, int stacks, bool smooth) : Primitives()
{
	type = PrimitivesTypes::PRIMITIVE_MYCYLINDER;

	this->baseRadius = baseRadius;
	this->topRadius = topRadius;
	this->height = height;
	this->sectors = sectors;
	this->sectorCount = sectorCounts;
	this->stacks = stacks;
	this->smooth = smooth;
	this->radius = this->topRadius;

	Initialize();
}

MyCylinder::~MyCylinder()
{
	unitCircleVertices.clear();
	vertices.clear();
	normals.clear();
	texCoords.clear();
	indices.clear();
	lineIndices.clear();
}

std::vector<float> MyCylinder::GetUnitCircleVertices()
{
	const float pi = 3.1415926f;
	float sectorStep = 2 * pi / sectorCount;
	float sectorAngle;

	std::vector<float> unitCircleVertices;
	for (int i = 0; i <= sectorCount; ++i)
	{
		sectorAngle = i * sectorStep;
		unitCircleVertices.push_back(cos(sectorAngle)); //x
		unitCircleVertices.push_back(sin(sectorAngle)); //y
		unitCircleVertices.push_back(0); //z
	}

	return unitCircleVertices;
}

void MyCylinder::BuildVerticalSmooth()
{
	// clear memory of prev arrays
	std::vector<float>().swap(vertices);
	std::vector<float>().swap(normals);
	std::vector<float>().swap(texCoords);

	// get unit circle vectors on XY-plane
	std::vector<float> unitVertices = GetUnitCircleVertices();

	//defining some useful vars
	static float sectorCount = 16.0f;
	static float radius = 2.0f;
	static float height = 5.0f;

	// put side vertices to arrays
	for (int i = 0; i < 2; ++i)
	{
		float h = -height / 2.0f + i * height;           // z value; -h/2 to h/2
		float t = 1.0f - i;                              // vertical tex coord; 1 to 0

		for (int j = 0, k = 0; j <= sectorCount; ++j, k += 3)
		{
			float ux = unitVertices[k];
			float uy = unitVertices[k + 1];
			float uz = unitVertices[k + 2];
			// position vector
			vertices.push_back(ux * radius);             // vx
			vertices.push_back(uy * radius);             // vy
			vertices.push_back(h);                       // vz
			// normal vector
			normals.push_back(ux);                       // nx
			normals.push_back(uy);                       // ny
			normals.push_back(uz);                       // nz
			// texture coordinate
			texCoords.push_back((float)j / sectorCount); // s
			texCoords.push_back(t);                      // t
		}
	}

	// the starting index for the base/top surface
	//NOTE: it is used for generating indices later
	int baseCenterIndex = (int)vertices.size() / 3;
	int topCenterIndex = baseCenterIndex + sectorCount + 1; // include center vertex

	// put base and top vertices to arrays
	for (int i = 0; i < 2; ++i)
	{
		float h = -height / 2.0f + i * height;           // z value; -h/2 to h/2
		float nz = -1 + i * 2;                           // z value of normal; -1 to 1

		// center point
		vertices.push_back(0);     vertices.push_back(0);     vertices.push_back(h);
		normals.push_back(0);      normals.push_back(0);      normals.push_back(nz);
		texCoords.push_back(0.5f); texCoords.push_back(0.5f);

		for (int j = 0, k = 0; j < sectorCount; ++j, k += 3)
		{
			float ux = unitVertices[k];
			float uy = unitVertices[k + 1];
			// position vector
			vertices.push_back(ux * radius);             // vx
			vertices.push_back(uy * radius);             // vy
			vertices.push_back(h);                       // vz
			// normal vector
			normals.push_back(0);                        // nx
			normals.push_back(0);                        // ny
			normals.push_back(nz);                       // nz
			// texture coordinate
			texCoords.push_back(-ux * 0.5f + 0.5f);      // s
			texCoords.push_back(-uy * 0.5f + 0.5f);      // t
		}
	}

	// generate CCW index list of cylinder triangles
	int k1 = 0;                         // 1st vertex index at base
	int k2 = sectorCount + 1;           // 1st vertex index at top

	// indices for the side surface
	for (int i = 0; i < sectorCount; ++i, ++k1, ++k2)
	{
		// 2 triangles per sector
		// k1 => k1+1 => k2
		indices.push_back(k1);
		indices.push_back(k1 + 1);
		indices.push_back(k2);

		// k2 => k1+1 => k2+1
		indices.push_back(k2);
		indices.push_back(k1 + 1);
		indices.push_back(k2 + 1);
	}

	// indices for the base surface
	//NOTE: baseCenterIndex and topCenterIndices are pre-computed during vertex generation
	//      please see the previous code snippet
	for (int i = 0, k = baseCenterIndex + 1; i < sectorCount; ++i, ++k)
	{
		if (i < sectorCount - 1)
		{
			indices.push_back(baseCenterIndex);
			indices.push_back(k + 1);
			indices.push_back(k);
		}
		else // last triangle
		{
			indices.push_back(baseCenterIndex);
			indices.push_back(baseCenterIndex + 1);
			indices.push_back(k);
		}
	}

	// indices for the top surface
	for (int i = 0, k = topCenterIndex + 1; i < sectorCount; ++i, ++k)
	{
		if (i < sectorCount - 1)
		{
			indices.push_back(topCenterIndex);
			indices.push_back(k);
			indices.push_back(k + 1);
		}
		else // last triangle
		{
			indices.push_back(topCenterIndex);
			indices.push_back(k);
			indices.push_back(topCenterIndex + 1);
		}
	}
}

void MyCylinder::DrawCylinder()
{
	if (App->editor->wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

void MyCylinder::Initialize()
{
	BuildVerticalSmooth();

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &bufferVertex);
	glBindBuffer(GL_ARRAY_BUFFER, bufferVertex);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &bufferIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
}

MySphere::MySphere(float radius, uint rings, uint sectors) : Primitives()
{
	type = PrimitivesTypes::PRIMITIVE_MYSPHERE;

	float const R = 1. / (float)(rings - 1);
	float const S = 1. / (float)(sectors - 1);
	int r, s;

	vertices.resize(rings * sectors * 3);
	normals.resize(rings * sectors * 3);
	texCoords.resize(rings * sectors * 3);

	std::vector<GLfloat>::iterator v = vertices.begin();
	std::vector<GLfloat>::iterator n = normals.begin();
	std::vector<GLfloat>::iterator t = texCoords.begin();
	for (r = 0; r < rings; r++)
	{
		for (s = 0; s < sectors; s++)
		{
			float const y = sin(-M_PI_2 + M_PI * r * R);
			float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
			float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

			*t++ = s * S;
			*t++ = r * R;

			*v++ = x * radius;
			*v++ = y * radius;
			*v++ = z * radius;

			*n++ = x;
			*n++ = y;
			*n++ = z;
		}
	}

	indices.resize(rings * sectors * 4);
	std::vector<GLushort>::iterator i = indices.begin();
	for (r = 0; r < rings; r++)
	{
		for (s = 0; s < sectors; s++)
		{
			*i++ = r * sectors + s;
			*i++ = r * sectors + (s + 1);
			*i++ = (r + 1) * sectors + (s + 1);
			*i++ = (r + 1) * sectors + s;
		}
	}

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &bufferVertex);
	glBindBuffer(GL_ARRAY_BUFFER, bufferVertex);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &bufferIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);
	glEnableVertexAttribArray(0);
}

MySphere::~MySphere()
{

}

void MySphere::DrawSphere()
{
	if (App->editor->wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindVertexArray(VAO);
	glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, NULL);
	glBindVertexArray(0);
}