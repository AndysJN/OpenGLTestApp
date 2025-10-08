#pragma once

#include <iostream>
#include <string>
#include <fstream>

#include <GL\glew.h>


class Shader
{
public:
	Shader();

	void CreateFromString(const char* VertexCode, const char* FragmentCode);

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();

	void UseShader();
	void ClearShader();

	~Shader();

private:
	GLuint ShaderID, UniformProjection, UniformModel;

	void CompileShader(const char* VertexCode, const char* FragmentCode);
	void AddShader(GLuint TheProgram, const char* ShaderCode, GLenum ShaderType);
};

