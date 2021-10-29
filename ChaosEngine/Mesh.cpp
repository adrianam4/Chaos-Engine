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
	if (App->editor->wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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

	//Disable states
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	if (App->editor->normals)
	{
		glColor4d(1.0f, 1.0f, 1.0f, 1.0f);
		glBegin(GL_LINES);
		for (int i = 0; i < vertices.size(); i += 3)
		{
			float3 vertex1(vertices[i].position.x, vertices[i].position.y, vertices[i].position.z);
			float3 vertex2(vertices[i + 1].position.x, vertices[i + 1].position.y, vertices[i + 1].position.z);
			float3 vertex3(vertices[i + 2].position.x, vertices[i + 2].position.y, vertices[i + 2].position.z);
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

	if (matrix != nullptr)
		glPopMatrix();
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

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_TEXTURE_COORD_ARRAY, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	aabb.SetNegativeInfinity();


	for (int a = 0; a < vertices.size(); a++) {
		vertices_aux.push_back(vertices[a].position);
		
	}
	aabb.Enclose((float3*)vertices_aux.data(), (size_t)vertices_aux.size());
	
	int a = 0;
}