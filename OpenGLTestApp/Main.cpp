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

GLuint VAO, VBO, Shader, UniformModel;

bool Direction = true;
float TriOffset = 0.0f;
float TriMaxOffset = 0.7f;
float TriIncrement = 0.005f;

float CurrentAngle = 0.0f;

// Vertex Shader creation
static const char* VShader = "												\n\
#version 330																\n\
																			\n\
layout (location = 0) in vec3 pos;											\n\
																			\n\
uniform mat4 Model;															\n\
																			\n\
void main()																	\n\
{																			\n\
	gl_Position = Model * vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0);		\n\
}";

// Fragment Shader creation

static const char* FShader = "												\n\
#version 330																\n\
																			\n\
out vec4 colour;															\n\
																			\n\
void main()																	\n\
{																			\n\
	colour = vec4(0.0 , 1.0 , 0.0 , 0.3);									\n\
}";

void CreateTriangle()
{
	GLfloat Vertices[] = {
		-1.f, -1.f, 0.f,
		1.f, -1.f, 0.f,
		0.f, 1.f, 0.f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices) /* sizeof GLfloat *9 = mas seguro en casos mas grandes, por el numero de vertices */,
			Vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
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

		// Clear Window
		glClearColor(0.f, 0.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw Call to Triangle (Se dira asi? GPT?)
		glUseProgram(Shader); // En caso de tener mas de un shader se podria usar un switch statement.

			// Actualizado para la ultima GLM <Antes glm::mat4 model> - Obligatorio inicializar.
			// Identity Matrix
			glm::mat4 model(1.0f);

			// El orden de las transformaciones sobre el modelo es MUY IMPORTANTE.
			model = glm::translate(model, glm::vec3(TriOffset, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(CurrentAngle), glm::vec3(0.0f, 0.0f, 1.0f));
			

			//glUniform1f(UniformModel, TriOffset);
			glUniformMatrix4fv(UniformModel, 1, GL_FALSE, glm::value_ptr(model)); //value_ptr get pointer to model.
		
			glBindVertexArray(VAO);

				glDrawArrays(GL_TRIANGLES, 0, 3);

			glBindVertexArray(0);

		glUseProgram(0);

		// Draw on the unseen buffer and swap it to be seen. (I think at least, review - Ask GPT?).
		glfwSwapBuffers(MainWindow);
	}

	return 0;
}