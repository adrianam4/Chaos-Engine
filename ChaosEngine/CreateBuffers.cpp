#include "CreateBuffers.h"

#include "GL/glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>

CreateBuffers::CreateBuffers(std::vector<float> vertices, std::vector<uint> indices)
{
	glGenBuffers(1, &bufferVertex);
	glBindBuffer(GL_ARRAY_BUFFER, bufferVertex);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &bufferIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices), indices.data(), GL_STATIC_DRAW);
}

CreateBuffers::CreateBuffers(float* vertices, uint* indices)
{
	glGenBuffers(1, &bufferVertex);
	glBindBuffer(GL_ARRAY_BUFFER, bufferVertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sizeof(float), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &bufferIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * sizeof(indices), indices, GL_STATIC_DRAW);
}

CreateBuffers::~CreateBuffers()
{
}
