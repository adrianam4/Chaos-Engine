#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "Primitives.h"

#include "glmath.h"
#include "MathGeoLib/src/MathGeoLib.h"
// ------------------------------------------------------------
Primitives::Primitives() : transform(IdentityMatrix), color(White), wire(false), axis(false), type(PrimitivesTypes::PRIMITIVE_MYPOINT)
{
	found = false;
}

// ------------------------------------------------------------
PrimitivesTypes Primitives::GetType() const
{
	return type;
}

int Primitives::TransformMatrix()
{
	for (int i = 0; i < App->scene->gameObjects.size(); i++)
	{
		if (App->scene->gameObjects[i]->id == id)
		{
			if (App->scene->gameObjects[i]->matrix != nullptr) 
			{
				glPushMatrix();
				glMultMatrixf(App->scene->gameObjects[i]->matrix);
				found = true;
				return i;
			}

		}
	}
}


int Primitives::TransformMatrixAABB()
{
	if (App->editor->objectSelected != nullptr)
	{
		for (int i = 0; i < App->editor->objectSelected->boundingBoxes.size(); i++)
		{
			if (App->editor->objectSelected->boundingBoxes[i]->matrix != nullptr)
			{
				glPushMatrix();
				glMultMatrixf(App->editor->objectSelected->boundingBoxes[i]->matrix);
				found = true;
				return i;
			}
		}
	}
}


std::vector<float3> Primitives::getVertex() {
	std::vector<float3> aux;
	return aux;
}

//// CUBE ================================================================================================================================================================================

BoundingBoxes::BoundingBoxes(float3 pos, float3 sca, float3 maxPoint, float3 minPoint): Primitives()
{
	type = PrimitivesTypes::PRIMITIVE_BOUNDINGBOX;
	position = pos;
	scale = sca;

	vertices.push_back({ (minPoint.x + position.x) * scale.x, (minPoint.y + position.y) * scale.y, (minPoint.z + position.z) * scale.z });
	vertices.push_back({ (maxPoint.x + position.x) * scale.x, (minPoint.y + position.y) * scale.y, (minPoint.z + position.z) * scale.z });
	vertices.push_back({ (maxPoint.x + position.x) * scale.x, (minPoint.y + position.y) * scale.y, (maxPoint.z + position.z) * scale.z });
	vertices.push_back({ (minPoint.x + position.x) * scale.x, (minPoint.y + position.y) * scale.y, (maxPoint.z + position.z) * scale.z });
	vertices.push_back({ (maxPoint.x + position.x) * scale.x, (maxPoint.y + position.y) * scale.y, (maxPoint.z + position.z) * scale.z });
	vertices.push_back({ (maxPoint.x + position.x) * scale.x, (maxPoint.y + position.y) * scale.y, (minPoint.z + position.z) * scale.z });
	vertices.push_back({ (minPoint.x + position.x) * scale.x, (maxPoint.y + position.y) * scale.y, (minPoint.z + position.z) * scale.z });
	vertices.push_back({ (minPoint.x + position.x) * scale.x, (maxPoint.y + position.y) * scale.y, (maxPoint.z + position.z) * scale.z });

	indices.push_back(5); indices.push_back(1); indices.push_back(0); indices.push_back(6); indices.push_back(5); indices.push_back(0); 
	indices.push_back(4); indices.push_back(2); indices.push_back(1); indices.push_back(5); indices.push_back(4); indices.push_back(1); 
	indices.push_back(4); indices.push_back(7); indices.push_back(2); indices.push_back(7); indices.push_back(3); indices.push_back(2); 
	indices.push_back(7); indices.push_back(0); indices.push_back(3); indices.push_back(6); indices.push_back(0); indices.push_back(7); 
	indices.push_back(0); indices.push_back(1); indices.push_back(3); indices.push_back(1); indices.push_back(2); indices.push_back(3);
	indices.push_back(7); indices.push_back(4); indices.push_back(6); indices.push_back(4); indices.push_back(5); indices.push_back(6);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float3), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

}
BoundingBoxes::~BoundingBoxes()
{
}
void BoundingBoxes::DrawCube()
{
	glLineWidth(3.0f);

	glColor4f(255, 0, 255, 255);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Enable states
	glEnableClientState(GL_VERTEX_ARRAY);

	if (matrix != nullptr)
	{
		glPushMatrix();
		glMultMatrixf(matrix);
	}
	//Buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Vertex
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	//Draw
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	//UnBind Buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Disable states
	glDisableClientState(GL_VERTEX_ARRAY);

	if (matrix != nullptr)
		glPopMatrix();

	glColor4f(255, 255, 255, 255);
}
MyCube::MyCube(float3 pos, float3 sca) : Primitives()
{
	position = pos;
	//scale = sca;
	scale = { 1,1,1 };
	type = PrimitivesTypes::PRIMITIVE_MYCUBE;

	vertices.push_back({ ((-1 + position.x) * sca.x), ((0 + position.y) * sca.y), (0 + position.z) * sca.z });
	vertices.push_back({ ((1 + position.x) * sca.x), ((0 + position.y) * sca.y), (0 + position.z) * sca.z });
	vertices.push_back({ ((1 + position.x) * sca.x), ((2 + position.y) * sca.y), (0 + position.z) * sca.z });
	vertices.push_back({ ((-1 + position.x) * sca.x), ((2 + position.y) * sca.y), (0 + position.z) * sca.z });

	vertices.push_back({ ((-1 + position.x) * sca.x), ((0 + position.y) * sca.y), (-2 + position.z) * sca.z });
	vertices.push_back({ ((1 + position.x) * sca.x), ((0 + position.y) * sca.y), (-2 + position.z) * sca.z });
	vertices.push_back({ ((1 + position.x) * sca.x), ((2 + position.y) * sca.y), (-2 + position.z) * sca.z });
	vertices.push_back({ ((-1 + position.x) * sca.x), ((2 + position.y) * sca.y), (-2+ position.z) * sca.z });
	
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

	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->rot.w = 0;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->rot.x = 0;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->rot.y = 0;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->rot.z = 0;

	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->sca.x = 1;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->sca.y = 1;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->sca.z = 1;

	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->trans.x = pos.x;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->trans.y = pos.y;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->trans.z = pos.z;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float3), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &TBO);
	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat), texCoords.data(), GL_STATIC_DRAW);

	aabb.SetNegativeInfinity();
	aabb.Enclose((float3*)vertices.data(), (size_t)vertices.size());
}

MyCube::~MyCube()
{
	vertices.clear();
	indices.clear();
}

std::vector<float3> MyCube::getVertex() 
{
	return vertices;
}

void MyCube::DrawCube()
{
	glLineWidth(3.0f);

	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//Enable states
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	int i = TransformMatrix();
	//Buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Vertex
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, TBO); // TexCoords
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	glBindTexture(GL_TEXTURE_2D, aTextureId); // Textures and Indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	aabb.SetNegativeInfinity();
	aabb.Enclose((float3*)vertices.data(), (size_t)vertices.size());
	
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

	if (found && App->scene->gameObjects[i]->matrix != nullptr)
		glPopMatrix();

}

//// PYRAMID ================================================================================================================================================================================
MyPyramid::MyPyramid(float3 pos, float3 sca) : Primitives()
{
	type = PrimitivesTypes::PRIMITIVE_MYPYRAMID;

	position = pos;
	//scale = sca;
	scale = { 1,1,1 };
	
	vertices.push_back({ ((-1 + position.x) * sca.x), ((0 + position.y) * sca.y),((-1 + position.z) * sca.z) });
	vertices.push_back({ ((1 + position.x) * sca.x), ((0 + position.y) * sca.y),((-1 + position.z) * sca.z) });
	vertices.push_back({ ((0 + position.x) * sca.x), ((2 + position.y) * sca.y),((0 + position.z) * sca.z) });
	vertices.push_back({ ((1 + position.x) * sca.x), ((0 + position.y) * sca.y),((1 + position.z) * sca.z) });
	vertices.push_back({ ((-1 + position.x) * sca.x), ((0 + position.y) * sca.y),((1 + position.z) * sca.z) });

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

	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->rot.w = 0;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->rot.x = 0;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->rot.y = 0;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->rot.z = 0;

	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->sca.x = 1;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->sca.y = 1;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->sca.z = 1;

	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->trans.x = pos.x;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->trans.y = pos.y;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->trans.z = pos.z;


	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float3), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &TBO);
	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat), texCoords.data(), GL_STATIC_DRAW);
	aabb.SetNegativeInfinity();
	aabb.Enclose((float3*)vertices.data(), (size_t)vertices.size());
}

MyPyramid::~MyPyramid()
{
	vertices.clear();
	indices.clear();
}
std::vector<float3> MyPyramid::getVertex()
{
	return vertices;
}
void MyPyramid::DrawPyramid() 
{
	//glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//Enable states
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	int i = TransformMatrix();

	//Buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Vertex
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, TBO); // TexCoords
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	glBindTexture(GL_TEXTURE_2D, aTextureId); // Textures and Indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	aabb.SetNegativeInfinity();
	aabb.Enclose((float3*)vertices.data(), (size_t)vertices.size());

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

	if (found && App->scene->gameObjects[i]->matrix != nullptr)
		glPopMatrix();
}


//// CYLINDER ================================================================================================================================================================================

MyCylinder::MyCylinder(float3 pos, float3 sca) : Primitives()
{
	type = PrimitivesTypes::PRIMITIVE_MYCYLINDER;

	position = pos;
	scale = {1,1,1};

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

MyCylinder::MyCylinder(float3 pos, float3 sca, float baseRadius, float topRadius, float height, int sectors, int sectorCounts, int stacks, bool smooth) : Primitives()
{
	type = PrimitivesTypes::PRIMITIVE_MYCYLINDER;

	position = pos;
	scale = { 1,1,1 };

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
std::vector<float3> MyCylinder::getVertex()
{
	return vertices;
}
std::vector<float3> MyCylinder::GetUnitCircleVertices()
{
    const float PI = 3.1415926f;
    float sectorStep = 2 * PI / sectorCount;
    float sectorAngle;  // radian

    std::vector<float3> unitCircleVertices;
    for(int i = 0; i <= sectorCount; ++i)
    {
        sectorAngle = i * sectorStep;
		unitCircleVertices.push_back({ cos(sectorAngle), sin(sectorAngle),0 });
    }

    return unitCircleVertices;
}

void MyCylinder::BuildVerticalSmooth()
{
	// clear memory of prev arrays
	std::vector<float3>().swap(vertices);
	std::vector<float>().swap(normals);
	std::vector<float>().swap(texCoords);

	// get unit circle vectors on XY-plane
	std::vector<float3> unitVertices = GetUnitCircleVertices();

	//defining some useful vars
	static float sectorCount = 16.0f;
	static float radius = 2.0f;
	static float height = 5.0f;

	// put side vertices to arrays
	for (int i = 0; i < 2; ++i)
	{
		float h = -height / 2.0f + i * height;           // z value; -h/2 to h/2
		float t = 1.0f - i;                              // vertical tex coord; 1 to 0

		for (int j = 0, k = 0; j <= sectorCount; ++j, ++k)
		{
			float ux = unitVertices[k].x;
			float uy = unitVertices[k].y;
			float uz = unitVertices[k].z;
			// position vector
			vertices.push_back({ ((ux * radius) + position.x) * scale.x ,((uy * radius) + position.y) * scale.x,(h + position.z) * scale.z });
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
	int baseCenterIndex = (int)vertices.size();
	int topCenterIndex = baseCenterIndex + sectorCount + 1; // include center vertex

	// put base and top vertices to arrays
	for (int i = 0; i < 2; ++i)
	{
		float h = -height / 2.0f + i * height;           // z value; -h/2 to h/2
		float nz = -1 + i * 2;                           // z value of normal; -1 to 1

		// center point
		vertices.push_back({ (0 + position.x) * scale.x ,(0 + position.y) * scale.y ,(h + position.z) * scale.z });
		normals.push_back(0);      normals.push_back(0);      normals.push_back(nz);
		texCoords.push_back(0.5f); texCoords.push_back(0.5f);

		for (int j = 0, k = 0; j < sectorCount; ++j, ++k)
		{
			float ux = unitVertices[k].x;
			float uy = unitVertices[k].y;
			// position vector
			vertices.push_back({ (ux * radius + position.x) * scale.x ,(uy * radius + position.y) * scale.y ,(h + position.z) * scale.z });
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

	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//Enable states
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	int i = TransformMatrix();

	//Buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Vertex
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, TBO); // TexCoords
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	glBindTexture(GL_TEXTURE_2D, aTextureId); // Textures and Indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	aabb.SetNegativeInfinity();
	aabb.Enclose((float3*)vertices.data(), (size_t)vertices.size());

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

	if (found && App->scene->gameObjects[i]->matrix != nullptr)
		glPopMatrix();
}


void MyCylinder::Initialize()
{
	BuildVerticalSmooth();

	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->rot.w = 0;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->rot.x = 0;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->rot.y = 0;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->rot.z = 0;

	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->sca.x = scale.x;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->sca.y = scale.y;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->sca.z = scale.z;

	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->trans.x = position.x;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->trans.y = position.y;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->trans.z = position.z;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float3), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &TBO);
	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat), texCoords.data(), GL_STATIC_DRAW);

	aabb.SetNegativeInfinity();
	aabb.Enclose((float3*)vertices.data(), (size_t)vertices.size());
}

//// SPHERE ================================================================================================================================================================================

MySphere::MySphere(float radius, int rings, int sectors, float3 pos, float3 sca) : Primitives()
{
	type = PrimitivesTypes::PRIMITIVE_MYSPHERE;
	
	position = pos;
	scale = { 1,1,1 };

	std::vector<float3>().swap(vertices);
	std::vector<float3>().swap(normals);
	std::vector<float>().swap(texCoords);

	float x, y, z, xy;                              // vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
	float s, t;                                     // vertex texCoord

	const float PI = 3.1415926f;

	float sectorStep = 2 * PI / sectors;
	float stackStep = PI / rings;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= rings; ++i)
	{
		stackAngle = M_PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
		xy = 1.0f * cosf(stackAngle);             // r * cos(u)
		z = 1.0f * sinf(stackAngle);              // r * sin(u)

		// add (sectorCount+1) vertices per stack
		// the first and last vertices have same position and normal, but different tex coords
		for (int j = 0; j <= sectors; ++j)
		{
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi

			// vertex position (x, y, z)
			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
			vertices.push_back({ (x + position.x)* scale.x, (y + position.y) * scale.y, (z + position.z) * scale.z });

			// normalized vertex normal (nx, ny, nz)
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			normals.push_back({ nx, ny, nz });

			// vertex tex coord (s, t) range between [0, 1]
			s = (float)j / sectors;
			t = (float)i / rings;
			texCoords.push_back(s);
			texCoords.push_back(t);
		}
	}

	std::vector<int> lineIndices;
	int k1, k2;
	for (int i = 0; i < rings; ++i)
	{
		k1 = i * (sectors + 1);     // beginning of current stack
		k2 = k1 + sectors + 1;      // beginning of next stack

		for (int j = 0; j < sectors; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding first and last stacks
			// k1 => k2 => k1+1
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 +1);
			}

			// k1+1 => k2 => k2+1
			if (i != (rings - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}

			// store indices for lines
			// vertical lines for all stacks, k1 => k2
			lineIndices.push_back(k1);
			lineIndices.push_back(k2);
			if (i != 0)  // horizontal lines except 1st stack, k1 => k+1
			{
				lineIndices.push_back(k1);
				lineIndices.push_back(k1 + 1);
			}
		}
	}

	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->rot.w = 0;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->rot.x = 0;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->rot.y = 0;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->rot.z = 0;

	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->sca.x = scale.x;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->sca.y = scale.y;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->sca.z = scale.z;

	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->trans.x = position.x;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->trans.y = position.y;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->trans.z = position.z;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) * 3, vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &TBO);
	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat), texCoords.data(), GL_STATIC_DRAW);

	aabb.SetNegativeInfinity();
	aabb.Enclose((float3*)vertices.data(), (size_t)vertices.size());
}

MySphere::~MySphere()
{

}
std::vector<float3> MySphere::getVertex()
{
	return vertices;
}
void MySphere::DrawSphere()
{
	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//Enable states
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	int i = TransformMatrix();

	//Buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Vertex
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, TBO); // TexCoords
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	glBindTexture(GL_TEXTURE_2D, aTextureId); // Textures and Indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	aabb.SetNegativeInfinity();
	aabb.Enclose((float3*)vertices.data(), (size_t)vertices.size());

	//Draw
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);

	//UnBind Buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_TEXTURE_COORD_ARRAY, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Disable states
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	if (found && App->scene->gameObjects[i]->matrix != nullptr)
		glPopMatrix();
}

//// PLANE ================================================================================================================================================================================

MyPlane::MyPlane(float3 pos, float3 sca) : Primitives()
{
	position = pos;
	scale = sca;

	type = PrimitivesTypes::PRIMITIVE_MYPLANE3D;

	vertices.push_back({ (1 + pos.x) * sca.x ,pos.y,(1 + pos.z) * sca.z });
	vertices.push_back({ (1 + pos.x) * sca.x ,pos.y, (pos.z - 1) * sca.z });
	vertices.push_back({ (pos.x - 1) * sca.x ,pos.y,(pos.z - 1) * sca.z });
	vertices.push_back({ (pos.x - 1) * sca.x ,pos.y,(1 + pos.z) * sca.z });

	indices.push_back(3);
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(3);
	indices.push_back(1);
	indices.push_back(2);

	texCoords.push_back(1);
	texCoords.push_back(0);
	texCoords.push_back(1);
	texCoords.push_back(1);
	texCoords.push_back(0);
	texCoords.push_back(1);
	texCoords.push_back(0);
	texCoords.push_back(0);

	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->rot.w = 0;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->rot.x = 0;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->rot.y = 0;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->rot.z = 0;

	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->sca.x = sca.x;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->sca.y = sca.y;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->sca.z = sca.z;

	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->trans.x = pos.x;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->trans.y = pos.y;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->trans.z = pos.z;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) * 3, vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &TBO);
	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat), texCoords.data(), GL_STATIC_DRAW);

	aabb.SetNegativeInfinity();
	aabb.Enclose((float3*)vertices.data(), (size_t)vertices.size());
}

MyPlane::~MyPlane()
{
	vertices.clear();
	indices.clear();
}
std::vector<float3> MyPlane::getVertex()
{
	return vertices;
}
void MyPlane::DrawPlane()
{
	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//Enable states
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	int i = TransformMatrix();

	//Buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Vertex
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, TBO); // TexCoords
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	glBindTexture(GL_TEXTURE_2D, aTextureId); // Textures and Indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	aabb.SetNegativeInfinity();
	aabb.Enclose((float3*)vertices.data(), (size_t)vertices.size());

	//Draw
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);

	//UnBind Buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_TEXTURE_COORD_ARRAY, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Disable states
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	if (found && App->scene->gameObjects[i]->matrix != nullptr)
		glPopMatrix();
}


Grid::Grid() : Primitives(), normal(0, 1, 0), constant(1)
{
	type = PrimitivesTypes::PRIMITIVE_MYPLANE;
}

Grid::Grid(float x, float y, float z, float d) : Primitives(), normal(x, y, z), constant(d)
{
	type = PrimitivesTypes::PRIMITIVE_MYPLANE;
}

void Grid::InnerRender() const
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

void Grid::DrawGrid() const
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