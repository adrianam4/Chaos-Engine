#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "Primitives.h"

#include "glmath.h"
#include "MathGeoLib/src/MathGeoLib.h"

#include "GL/glew.h"

#include "DevIL/include/IL/il.h"
#include "DevIL/include/IL/ilu.h"
#include "DevIL/include/IL/ilut.h"
// ------------------------------------------------------------
Primitives::Primitives() : transform(IdentityMatrix), color(White), wire(false), axis(false), type(PrimitivesTypes::PRIMITIVE_MYPOINT)
{
}

// ------------------------------------------------------------
PrimitivesTypes Primitives::GetType() const
{
	return type;
}

//// CUBE ================================================================================================================================================================================
MyCube::MyCube(float3 pos, float3 sca) : Primitives()
{
	position = pos;
	scale = sca;

	type = PrimitivesTypes::PRIMITIVE_MYCUBE;

	vertices.push_back((-1 + position.x) * scale.x); vertices.push_back((0 + position.y) * scale.y); vertices.push_back((0 + position.z) * scale.z);
	vertices.push_back((1 + position.x) * scale.x); vertices.push_back((0 + position.y) * scale.y); vertices.push_back((0 + position.z) * scale.z);
	vertices.push_back((1 + position.x) * scale.x); vertices.push_back((2 + position.y) * scale.y); vertices.push_back((0 + position.z) * scale.z);
	vertices.push_back((-1 + position.x) * scale.x); vertices.push_back((2 + position.y) * scale.y); vertices.push_back((0 + position.z) * scale.z);

	vertices.push_back((-1 + position.x) * scale.x); vertices.push_back((0 + position.y) * scale.y); vertices.push_back((-2 + position.z) * scale.z);
	vertices.push_back((1 + position.x) * scale.x); vertices.push_back((0 + position.y) * scale.y); vertices.push_back((-2 + position.z) * scale.z);
	vertices.push_back((1 + position.x) * scale.x); vertices.push_back((2 + position.y) * scale.y); vertices.push_back((-2 + position.z) * scale.z);
	vertices.push_back((-1 + position.x) * scale.x); vertices.push_back((2 + position.y) * scale.y); vertices.push_back((-2 + position.z) * scale.z);
	
	indices.push_back(0); indices.push_back(1); indices.push_back(2); indices.push_back(0); indices.push_back(2); indices.push_back(3);
	indices.push_back(6); indices.push_back(5); indices.push_back(4); indices.push_back(7); indices.push_back(6); indices.push_back(4);
	indices.push_back(1); indices.push_back(5); indices.push_back(6); indices.push_back(1); indices.push_back(6); indices.push_back(2);
	indices.push_back(4); indices.push_back(0); indices.push_back(3); indices.push_back(4); indices.push_back(3); indices.push_back(7);
	indices.push_back(3); indices.push_back(2); indices.push_back(6); indices.push_back(3); indices.push_back(6); indices.push_back(7);
	indices.push_back(5); indices.push_back(1); indices.push_back(0); indices.push_back(4); indices.push_back(5); indices.push_back(0);

	/* NOT CORRECT TEX COORDS */	
	texCoords.push_back(1); texCoords.push_back(1);
	texCoords.push_back(0); texCoords.push_back(1);
	texCoords.push_back(0); texCoords.push_back(0);
	texCoords.push_back(1); texCoords.push_back(0);

	texCoords.push_back(1); texCoords.push_back(1);
	texCoords.push_back(0); texCoords.push_back(1);
	texCoords.push_back(0); texCoords.push_back(0);
	texCoords.push_back(1); texCoords.push_back(0);


	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &TBO);
	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat), texCoords.data(), GL_STATIC_DRAW);

	ILboolean success;
	ILuint imageID;
	int width;
	int height;

	glGenTextures(1, &aTextureId);
	glBindTexture(GL_TEXTURE_2D, aTextureId);

	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	success = ilLoadImage("Assets/Lenna_(test_image).png");

	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);

	if (success) 
	{
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		if (!success)
		{
			std::cout << "Could not convert image :: " << "wood.png" << std::endl;
			ilDeleteImages(1, &imageID);
		}
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());

	glBindTexture(GL_TEXTURE_2D, 0);
}

MyCube::~MyCube()
{
	vertices.clear();
	indices.clear();
}

void MyCube::DrawCube()
{
	glLineWidth(3.0f);

	if (App->editor->wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//Enable states
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//Buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Vertex
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, TBO); // TexCoords
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	glBindTexture(GL_TEXTURE_2D, aTextureId); // Textures and Indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	
	//Draw
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); 

	//UnBind Buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_TEXTURE_COORD_ARRAY, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Disable states
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

//// PYRAMID ================================================================================================================================================================================
MyPyramid::MyPyramid(float3 pos, float3 sca) : Primitives()
{
	type = PrimitivesTypes::PRIMITIVE_MYPYRAMID;

	position = pos;
	scale = sca;

	vertices.push_back((-1 + position.x) * scale.x); vertices.push_back((0 + position.y) * scale.y); vertices.push_back((-1 + position.z) * scale.z);
	vertices.push_back((1 + position.x) * scale.x);vertices.push_back((0 + position.y) * scale.y);vertices.push_back((-1 + position.z) * scale.z);
	vertices.push_back((0 + position.x) * scale.x); vertices.push_back((2 + position.y) * scale.y); vertices.push_back((0 + position.z) * scale.z);
	vertices.push_back((1 + position.x) * scale.x);vertices.push_back((0 + position.y) * scale.y);vertices.push_back((1 + position.z) * scale.z);
	vertices.push_back((-1 + position.x) * scale.x);vertices.push_back((0 + position.y) * scale.y);vertices.push_back((1 + position.z) * scale.z);

	indices.push_back(2); indices.push_back(1); indices.push_back(0);
	indices.push_back(2); indices.push_back(3); indices.push_back(1);
	indices.push_back(4); indices.push_back(3); indices.push_back(2);
	indices.push_back(4); indices.push_back(2); indices.push_back(0);
	indices.push_back(0); indices.push_back(1); indices.push_back(3);
	indices.push_back(0); indices.push_back(3); indices.push_back(4);

	/* NOT CORRECT TEX COORDS */
	texCoords.push_back(0); texCoords.push_back(0);
	texCoords.push_back(1); texCoords.push_back(0);
	texCoords.push_back(0.5); texCoords.push_back(1);
	texCoords.push_back(0); texCoords.push_back(0);
	texCoords.push_back(1); texCoords.push_back(0);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &TBO);
	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat), texCoords.data(), GL_STATIC_DRAW);

	ILboolean success;
	ILuint imageID;
	int width;
	int height;

	glGenTextures(1, &aTextureId);
	glBindTexture(GL_TEXTURE_2D, aTextureId);

	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	success = ilLoadImage("Assets/Lenna_(test_image).png");

	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);

	if (success)
	{
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		if (!success)
		{
			std::cout << "Could not convert image :: " << "wood.png" << std::endl;
			ilDeleteImages(1, &imageID);
		}
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());

	glBindTexture(GL_TEXTURE_2D, 0);
}

MyPyramid::~MyPyramid()
{
	vertices.clear();
	indices.clear();
}
 
void MyPyramid::DrawPyramid() 
{
	//glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

	if (App->editor->wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//Enable states
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//Buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Vertex
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, TBO); // TexCoords
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	glBindTexture(GL_TEXTURE_2D, aTextureId); // Textures and Indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	//Draw
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	//UnBind Buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_TEXTURE_COORD_ARRAY, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Disable states
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

//// CYLINDER ================================================================================================================================================================================

MyCylinder::MyCylinder(float3 pos, float3 sca) : Primitives()
{
	type = PrimitivesTypes::PRIMITIVE_MYCYLINDER;

	position = pos;
	scale = sca;

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
			vertices.push_back((ux * radius) + position.x);             // vx
			vertices.push_back((uy * radius) + position.y);            // vy
			vertices.push_back(h + position.z);                       // vz
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
		vertices.push_back(0 + position.x);     vertices.push_back(0 + position.y);     vertices.push_back(h + position.z);
		normals.push_back(0);      normals.push_back(0);      normals.push_back(nz);
		texCoords.push_back(0.5f); texCoords.push_back(0.5f);

		for (int j = 0, k = 0; j < sectorCount; ++j, k += 3)
		{
			float ux = unitVertices[k];
			float uy = unitVertices[k + 1];
			// position vector
			vertices.push_back(ux * radius + position.x);             // vx
			vertices.push_back(uy * radius + position.y);             // vy
			vertices.push_back(h + position.z);                       // vz
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
	//glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

	if (App->editor->wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindBuffer(VBO, 0);
	glBindBuffer(EBO, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
}

void MyCylinder::Initialize()
{
	BuildVerticalSmooth();

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

//// SPHERE ================================================================================================================================================================================

MySphere::MySphere(float3 pos,float3 sca,float radius, uint rings, uint sectors) : Primitives()
{
	type = PrimitivesTypes::PRIMITIVE_MYSPHERE;

	position = pos;
	scale = sca;

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

			//TODO: FIX PIXELS
			//TODO: WHERE DEFINE AND WORK WITH POSITIONS
			//TODO: HOW DOES MATRIX WORK

			*t++ = s * S;
			*t++ = r * R;

			*v++ = (x * radius) + position.x;
			*v++ = (y * radius) + position.y;
			*v++ = (z * radius) + position.z;

			*n++ = x + position.x;
			*n++ = y + position.y;
			*n++ = z + position.z;
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

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
	
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

	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, 0);
	glBindBuffer(VBO, 0);
	glBindBuffer(EBO, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
}

//// PLANE ================================================================================================================================================================================

MyPlane::MyPlane() : Primitives(), normal(0, 1, 0), constant(1)
{
	type = PrimitivesTypes::PRIMITIVE_MYPLANE;
}

MyPlane::MyPlane(float x, float y, float z, float d) : Primitives(), normal(x, y, z), constant(d)
{
	type = PrimitivesTypes::PRIMITIVE_MYPLANE;
}

void MyPlane::InnerRender() const
{
	glLineWidth(1.0f);

	glBegin(GL_LINES);

	float d = 200.0f;

	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}

	glEnd();
}

void MyPlane::DrawPlane() const
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

//// LINE ================================================================================================================================================================================
MyLine::MyLine() : Primitives(), origin(0, 0, 0), destination(1, 1, 1)
{
	type = PrimitivesTypes::PRIMITIVE_MYLINE;
}

MyLine::MyLine(float x, float y, float z) : Primitives(), origin(0, 0, 0), destination(x, y, z)
{
	type = PrimitivesTypes::PRIMITIVE_MYLINE;
}

void MyLine::DrawLine() const
{
	glLineWidth(2.0f);

	glBegin(GL_LINES);

	glVertex3f(origin.x, origin.y, origin.z);
	glVertex3f(destination.x, destination.y, destination.z);

	glEnd();

	glLineWidth(1.0f);
}