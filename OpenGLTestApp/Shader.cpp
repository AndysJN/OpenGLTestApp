#include "Shader.h"

Shader::Shader() : ShaderID(0), UniformModel(0), UniformProjection(0)
{

}

void Shader::CreateFromString(const char* VertexCode, const char* FragmentCode)
{
	CompileShader(VertexCode, FragmentCode);
}

void Shader::CompileShader(const char* VertexCode, const char* FragmentCode)
{
	ShaderID = glCreateProgram();

	if (!ShaderID) //Can it receive id 0 ?
	{
		printf("Error creating Shader Program");
		return;
	}

	AddShader(ShaderID, VertexCode, GL_VERTEX_SHADER);
	AddShader(ShaderID, FragmentCode, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024]{};

	glLinkProgram(ShaderID);
	glGetProgramiv(ShaderID, GL_LINK_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(ShaderID, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(ShaderID);
	glGetProgramiv(ShaderID, GL_VALIDATE_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(ShaderID, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

	UniformProjection = glGetUniformLocation(ShaderID, "Projection");
	UniformModel = glGetUniformLocation(ShaderID, "Model");
}

GLuint Shader::GetProjectionLocation()
{
	return UniformProjection;
}

GLuint Shader::GetModelLocation()
{
	return UniformModel;
}

void Shader::UseShader()
{
	if (ShaderID == 0)
	{
		printf("Shader = 0, Error");
		return;
	}

	glUseProgram(ShaderID);
}

void Shader::ClearShader()
{
	//Liberar memoria de la tarjeta grafica
	if (ShaderID != 0)
	{
		glDeleteProgram(ShaderID);
		ShaderID = 0;
	}
}

void Shader::AddShader(GLuint TheProgram, const char* ShaderCode, GLenum ShaderType)
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

Shader::~Shader()
{
	ClearShader();
}