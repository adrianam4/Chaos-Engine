#include "Application.h"
#include "Mesh.h"

#include "Glew/include/GL/glew.h"

#include "DevIL/include/IL/il.h"
#include "DevIL/include/IL/ilu.h"
#include "DevIL/include/IL/ilut.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Textures> textures, std::vector<float> texCoord)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	this->texCoords = texCoord;

	setupMesh();
}

void Mesh::Draw(float* matrix)
{
	// THIS IS FOR DRAWING TEXTURES ----------------------------------------------------- 

	//unsigned int diffuseNr = 1;
	//unsigned int specularNr = 1;
	//for (unsigned int i = 0; i < textures.size(); i++)
	//{
	//	glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
	//	// retrieve texture number (the N in diffuse_textureN)
	//	string number;
	//	string name = textures[i].type;
	//	if (name == "texture_diffuse")
	//		number = std::to_string(diffuseNr++);
	//	else if (name == "texture_specular")
	//		number = std::to_string(specularNr++);

	//	shader.setFloat(("material." + name + number).c_str(), i);
	//	glBindTexture(GL_TEXTURE_2D, textures[i].id);
	//}
	//glActiveTexture(GL_TEXTURE0);
	// ------------------------------------------------------------------------------------

	// draw mesh
	//Enable states
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	if (matrix != nullptr)
	{
		glPushMatrix();
		glMultMatrixf(matrix);
	}

	//Buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Vertex
	glVertexPointer(3, GL_FLOAT, 8 * sizeof(float), NULL);

	glBindBuffer(GL_ARRAY_BUFFER, TBO); // TexCoords
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	glBindTexture(GL_TEXTURE_2D, textureId); // Textures and Indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	//Draw
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	//UnBind Buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_TEXTURE_COORD_ARRAY, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (matrix != nullptr)
		glPopMatrix();

	//Disable states
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	if (App->editor->normals)
	{
		glColor4d(1.0f, 1.0f, 1.0f, 1.0f);
		glBegin(GL_LINES);
		for (int i = 0; i < vertices.size(); i += 3)
		{
			float3 vertex1(vertices[i].Position.x, vertices[i].Position.y, vertices[i].Position.z);
			float3 vertex2(vertices[i + 1].Position.x, vertices[i + 1].Position.y, vertices[i + 1].Position.z);
			float3 vertex3(vertices[i + 2].Position.x, vertices[i + 2].Position.y, vertices[i + 2].Position.z);
			float3 avgVertex((vertex1.x + vertex2.x + vertex3.x) / 3, (vertex1.y + vertex2.y + vertex3.y) / 3, (vertex1.z + vertex2.z + vertex3.z) / 3);

			float3 line = -(vertex1 - vertex2);
			float3 line2 = vertex2 - vertex3;

			float3 normal = math::Cross(line2, line);
			normal.Normalize();

			glVertex3f(avgVertex.x, avgVertex.y, avgVertex.z);
			glVertex3f(avgVertex.x + normal.x, avgVertex.y + normal.y, avgVertex.z + normal.z);
		}
		glEnd();
	}
}

void Mesh::setupMesh()
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &TBO);
	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat), texCoords.data(), GL_STATIC_DRAW);

	ILboolean success;
	ILuint imageID;
	int width;
	int height;

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

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

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_TEXTURE_COORD_ARRAY, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}