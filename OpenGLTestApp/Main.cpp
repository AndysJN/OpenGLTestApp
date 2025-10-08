#include <iostream>
#include <string>
#include <cmath>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

const GLint WIDTH = 800, HEIGHT = 600;
const float ToRadias = 3.14159265f / 180.0f; // La funcion de rotacion usa radianes y no grados. (0 - 2PI) [Convertira el numero a Radianes] <Lo remplace con la funcion de GLM>

GLuint VAO, VBO, IBO /*Index buffer Object - Index Draw*/, Shader, UniformModel;

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
static const char* VShader = "												\n\
#version 330																\n\
																			\n\
layout (location = 0) in vec3 pos;											\n\
																			\n\
out vec4 VertexColour;														\n\
																			\n\
uniform mat4 Model;															\n\
																			\n\
void main()																	\n\
{																			\n\
	gl_Position = Model * vec4(pos, 1.0);									\n\
	VertexColour = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);						\n\
}";

// Fragment Shader creation

static const char* FShader = "												\n\
#version 330																\n\
																			\n\
in vec4 VertexColour;														\n\
																			\n\
out vec4 colour;															\n\
																			\n\
void main()																	\n\
{																			\n\
	colour = VertexColour;													\n\
}";

void CreateTriangle()
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

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices) /* sizeof GLfloat *9 = mas seguro en casos mas grandes, por el numero de vertices */,
			Vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //Unbind this AFTER the VAO Unbind (Porque no queremos que esta operacion se registre en el VAO). - Revisar igual.
}

void AddShader(GLuint TheProgram, const char* ShaderCode, GLenum ShaderType)
{
	GLuint TheShader = glCreateShader(ShaderType);

	const GLchar* TheCode[1];
	TheCode[0] = ShaderCode;

	GLint CodeLenght[1];
	CodeLenght[0] = strlen(ShaderCode);

	glShaderSource(TheShader, 1, TheCode, CodeLenght);
	glCompileShader(TheShader);

	GLint result = 0;
	GLchar eLog[1024]{};

	glGetShaderiv(TheShader, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		glGetShaderInfoLog(TheShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the '%d' shader: '%s'\n", ShaderType, eLog);
		return;
	}

	glAttachShader(TheProgram, TheShader);
}

void CompileShaders()
{
	Shader = glCreateProgram();

	if (!Shader) //Can it receive id 0 ?
	{
		printf("Error creating Shader Program");
		return;
	}

	AddShader(Shader, VShader, GL_VERTEX_SHADER);
	AddShader(Shader, FShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024]{};

	glLinkProgram(Shader);
	glGetProgramiv(Shader, GL_LINK_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(Shader, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(Shader);
	glGetProgramiv(Shader, GL_VALIDATE_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(Shader, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

	UniformModel = glGetUniformLocation(Shader, "Model");
}

int main()
{
	//Initialise GLFW
	if (!glfwInit())
	{
		printf("GLFW Initialisation failed!");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window properties
	// OpenGL Version -- 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Core profile = No Backwards compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* MainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
	
	if (!MainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get Buffer Size Information (Related to viewport and portion used)
	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(MainWindow, &BufferWidth, &BufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(MainWindow);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Glew Initialisation failed!");
		glfwDestroyWindow(MainWindow);
		glfwTerminate();
		return 1;
	}

	// Enable depth test - Which triangles are drawn in top of others
	glEnable(GL_DEPTH_TEST);

	// Setup viewport size
	glViewport(0, 0, BufferWidth, BufferHeight);

	// Create Triangle

	CreateTriangle();
	CompileShaders();

	// Loop until window closed - click close "x"?
	while (!glfwWindowShouldClose(MainWindow))
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

		// Draw Call to Triangle (Se dira asi? GPT?)
		glUseProgram(Shader); // En caso de tener mas de un shader se podria usar un switch statement.

			// Actualizado para la ultima GLM <Antes glm::mat4 model> - Obligatorio inicializar.
			// Identity Matrix
			glm::mat4 Model(1.0f);

			// El orden de las transformaciones sobre el modelo es MUY IMPORTANTE.
			//Model = glm::translate(Model, glm::vec3(TriOffset, 0.0f, 0.0f));
			Model = glm::rotate(Model, glm::radians(CurrentAngle), glm::vec3(0.0f, 1.0f, 0.0f));
			Model = glm::scale(Model, glm::vec3(0.4f, 0.4f, 1.0f));

			//glUniform1f(UniformModel, TriOffset);
			glUniformMatrix4fv(UniformModel, 1, GL_FALSE, glm::value_ptr(Model)); //value_ptr get pointer to model.
		
			glBindVertexArray(VAO);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

				glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //Desbindeo esto antes o despues del VAO ?
			
			glBindVertexArray(0);

		glUseProgram(0);

		// Draw on the unseen buffer and swap it to be seen. (I think at least, review - Ask GPT?).
		glfwSwapBuffers(MainWindow);
	}

	return 0;
}