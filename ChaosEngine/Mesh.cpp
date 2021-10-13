#include "Application.h"
#include "Mesh.h"

#include "Glew/include/GL/glew.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Textures> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setupMesh();
}

void Mesh::Draw()
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
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

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

	glBindVertexArray(0);
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	//vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	//vertex tex coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,texCoords));

	glBindVertexArray(0);
}
