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
MyCube::MyCube(float x, float y, float z) : Primitives()
{

	type = PrimitivesTypes::PRIMITIVE_MYCUBE;

	static GLfloat vertices[] =
	{
		-1 + x, 0 + y, 0 + z,
		1 + x, 0 + y, 0 + z,
		1 + x, 2 + y, 0 + z,
		-1 + x, 2 + y, 0 + z,

		-1 + x, 0 + y, -2 + z,
		1 + x, 0 + y, -2 + z,
		1 + x, 2 + y, -2 + z,
		-1 + x, 2 + y, -2 + z
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
MyPyramid::MyPyramid(float x, float y, float z) : Primitives()
{
	type = PrimitivesTypes::PRIMITIVE_MYPYRAMID;

	static GLfloat pyramid_vertices[] =
	{
		0 + x, 0 + y, -1 + z,
		1 + x, 0 + y, 0 + z,
		0 + x, 0 + y, 1 + z,
		-1 + x, 0 + y, 0 + z,

		0 + x, 2 + y, 0 + z
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

MyCylinder::MyCylinder() : Primitives()
{
	baseRadius = 2.0f;
	topRadius = 2.0f;
	height = 5.0f;
	sectors = 16;
	stacks = 5;
	smooth = true;

	Initialize();
}

MyCylinder::MyCylinder(float baseRadius, float topRadius, float height, int sectors, int stacks, bool smooth) : Primitives()
{
	this->baseRadius = baseRadius;
	this->topRadius = topRadius;
	this->height = height;
	this->sectors = sectors;
	this->stacks = stacks;
	this->smooth = smooth;

	Initialize();
}

MyCylinder::~MyCylinder()
{
}

std::vector<float> MyCylinder::GetUnitCircleVertices()
{
	const float pi = 3.1415926f;
	float sectorCount = 16.0f;
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

void MyCylinder::Draw()
{
	glBindBuffer(GL_VERTEX_ARRAY, buffer);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void MyCylinder::Initialize()
{
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_VERTEX_ARRAY, buffer);
	BuildVerticalSmooth();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);
	glVertexPointer(3, GL_FLOAT, 0, vertices.data());
	glNormalPointer(GL_FLOAT, 0, normals.data());
	glTexCoordPointer(3, GL_FLOAT, 0, texCoords.data());
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