#pragma once

#include "MathGeoLib/src/MathGeoLib.h"

struct Vertex
{
	float3 position;
	float3 normal;
	float2 texCoords;
};

struct Textures
{
	unsigned int id;
	std::string type; // or const char *
	std::string path; // or const char *
};

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<float3> vertices_aux;
	std::vector<unsigned int> indices;
	std::vector<Textures> textures;
	std::vector<float> texCoords;
	AABB aabb;
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Textures> textures, std::vector<float> texCoord);
	void Draw(float* matrix);

	unsigned int VBO, EBO, TBO, textureId;
	void setupMesh();
};