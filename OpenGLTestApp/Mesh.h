#pragma once

#include <GL\glew.h>
class Mesh
{

public:
	Mesh();

	void CreateMesh(GLfloat* Vertices, unsigned int* Indices, unsigned int NumOfVertices, unsigned int NumOfIndices);
	void RenderMesh();
	void ClearMesh();

	~Mesh();

private:
	GLuint VAO, VBO, IBO /*Index buffer Object - Index Draw*/;
	GLsizei IndexCount;

};

