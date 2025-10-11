#include "Mesh.h"
#include <iostream>

Mesh::Mesh() : VAO (0), VBO (0), IBO (0), IndexCount (0)
{

}

void Mesh::CreateMesh(GLfloat* Vertices, unsigned int* Indices, unsigned int NumOfVertices, unsigned int NumOfIndices)
{
	IndexCount = NumOfIndices;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * NumOfIndices, Indices, GL_STATIC_DRAW);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices[0]) * NumOfVertices, Vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertices[0]) * 5, 0);
		glEnableVertexAttribArray(0);
		//Texture coordinates at 1 -- stride 5, offset of first item to 3. c-cast to Void*
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertices[0]) * 5, (void*)(sizeof(Vertices[0]) * 3));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //Unbind this AFTER the VAO Unbind (Porque no queremos que esta operacion se registre en el VAO). - Revisar igual.
}

void Mesh::RenderMesh()
{
	if (!VAO) {
		printf("No Hay VAO");
		return;
	}

	glBindVertexArray(VAO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

		glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //Desbindeo esto antes o despues del VAO ?

	glBindVertexArray(0);
}

void Mesh::ClearMesh()
{
	//Liberar memoria en la placa grafica
	if (IBO != 0)
	{
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}

	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}

	IndexCount = 0;
}

Mesh::~Mesh()
{
	//Asegurarse de liberar memoria si el objeto es destruido/cae fuera de scope.
	ClearMesh();
}
