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
	void CreateFromFiles(const char* VertexLocation, const char* FragmentLocation); //Podria pasar un array.

	std::string ReadFile(const char* FileLocation);

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();

	void UseShader();
	void ClearShader();

	~Shader();

private:
	GLuint ShaderID, UniformProjection, UniformModel, UniformView;

	void CompileShader(const char* VertexCode, const char* FragmentCode);
	void AddShader(GLuint TheProgram, const char* ShaderCode, GLenum ShaderType);
};

