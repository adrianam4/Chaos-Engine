#pragma once

#include "MathGeoLib/src/MathGeoLib.h"

struct Vertex
{
	float3 Position;
	float3 Normal;
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
	std::vector<unsigned int> indices;
	std::vector<Textures> textures;
	std::vector<float> texCoords;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Textures> textures, std::vector<float> texCoord);
	void Draw();
private:
	unsigned int VBO, EBO, TBO, textureId;
	void setupMesh();
};