#include <iostream>
#include <string>
#include <cmath>
#include <vector>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include "GLWindow.h"
#include "Mesh.h"
#include "Shader.h"

const GLint WIDTH = 800, HEIGHT = 600;
const float ToRadias = 3.14159265f / 180.0f; // La funcion de rotacion usa radianes y no grados. (0 - 2PI) [Convertira el numero a Radianes] <Lo remplace con la funcion de GLM>

GLWindow MainWindow;
std::vector<Mesh*> MeshList;
std::vector<Shader> ShaderList;

bool Direction = true;
float TriOffset = 0.0f;
float TriMaxOffset = 0.7f;
float TriIncrement = 0.005f;

float CurrentAngle = 0.0f;

bool SizeDirection = true;
float CurrentSize = 0.4f;
float MaxSize = 0.8f;
float MinSize = 0.2f;

// Vertex Shader creation
static const char* VShader = "Shaders/Shader.vert";

// Fragment Shader creation

static const char* FShader = "Shaders/Shader.frag";

void CreateObjects()
{
	unsigned int Indices[] = {
		0, 3, 1, //1 side
		1, 3, 2, //2 side
		2, 3, 0, //front face
		0, 1, 2  //base
	};

	GLfloat Vertices[] = {
		-1.f, -1.f, 0.f, //Bottom Left [0]
		0.0f, -1.0f, 1.f, //Background - Depth [1]
		1.f, -1.f, 0.f, //Bottom Right [2]
		0.f, 1.f, 0.f //Top [3]
	};

	Mesh* Obj1 = new Mesh;
	Obj1->CreateMesh(Vertices, Indices, 12, 12); //Numeros magicos por ahora.
	MeshList.push_back(Obj1);

	Mesh* Obj2 = new Mesh;
	Obj1->CreateMesh(Vertices, Indices, 12, 12);
	MeshList.push_back(Obj2);
}

void CreateShaders()
{
	Shader* Shader1 = new Shader;
	Shader1->CreateFromFiles(VShader, FShader);
	ShaderList.push_back(*Shader1); //Usar el puntero directamente y hacer un vector de punteros y no de objetos?
}

int main()
{
	MainWindow = GLWindow(800, 600);
	MainWindow.Initialise();

	// Create Objects

	CreateObjects();
	CreateShaders();

	GLuint UniformProjection = 0, UniformModel = 0;
	glm::mat4 Projection = glm::perspective(45.0f, MainWindow.GetBufferWidth() / MainWindow.GetBufferHeight(), 0.1f, 100.0f);

	// Loop until window closed - click close "x"?
	while (!MainWindow.GetShouldClose())
	{
		// Get + Hanlde user input events
		glfwPollEvents();

		// Cambiar direccion?
		if (Direction)
		{
			TriOffset += TriIncrement;
		}
		else
		{
			TriOffset -= TriIncrement;
		}

		if (abs(TriOffset) >= TriMaxOffset)
		{
			Direction = !Direction;
		}

		CurrentAngle += 0.1f;
		if (CurrentAngle >= 360)
		{
			CurrentAngle -= 360;
		}

		if (SizeDirection)
		{
			CurrentSize += 0.001f;
		}
		else
		{
			CurrentSize -= 0.001f;
		}

		if (CurrentSize >= MaxSize || CurrentSize <= MinSize)
		{
			SizeDirection = !SizeDirection;
		}

		// Clear Window
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //bitwise to combine both bits.

		// Draw Call to Object (Se dira asi? GPT?)
		ShaderList[0].UseShader();
		UniformModel = ShaderList[0].GetModelLocation();
		UniformProjection = ShaderList[0].GetProjectionLocation();

			// Actualizado para la ultima GLM <Antes glm::mat4 model> - Obligatorio inicializar.
			// Identity Matrix
			glm::mat4 Model(1.0f);

			// El orden de las transformaciones sobre el modelo es MUY IMPORTANTE.
			Model = glm::translate(Model, glm::vec3(TriOffset, 0.0f, -2.5f));
			//Model = glm::rotate(Model, glm::radians(CurrentAngle), glm::vec3(0.0f, 1.0f, 0.0f));
			Model = glm::scale(Model, glm::vec3(0.4f, 0.4f, 1.0f));
			
			glUniformMatrix4fv(UniformModel, 1, GL_FALSE, glm::value_ptr(Model)); //value_ptr get pointer to model.
			glUniformMatrix4fv(UniformProjection, 1, GL_FALSE, glm::value_ptr(Projection));

			MeshList[0]->RenderMesh();

			Model = glm::mat4(1.0f);

			Model = glm::translate(Model, glm::vec3(-TriOffset, 1.0f, -2.5f));
			//Model = glm::rotate(Model, glm::radians(CurrentAngle), glm::vec3(0.0f, 1.0f, 0.0f));
			Model = glm::scale(Model, glm::vec3(0.4f, 0.4f, 1.0f));
			
			glUniformMatrix4fv(UniformModel, 1, GL_FALSE, glm::value_ptr(Model)); //value_ptr get pointer to model.
		
			MeshList[0]->RenderMesh();


		glUseProgram(0); //Remplazar con una static function ?

		// Draw on the unseen buffer and swap it to be seen. (I think at least, review - Ask GPT?).
		MainWindow.SwapBuffers();
	}

	return 0;
}