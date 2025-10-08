#pragma once

#include <stdio.h>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

class GLWindow
{
public:
	GLWindow();
	GLWindow(GLint WindowWidth, GLint WindowHeight);

	int Initialise();

	~GLWindow();

	inline GLfloat GetBufferWidth() const { return BufferWidth; }
	inline GLfloat GetBufferHeight() const { return BufferHeight; }

	bool GetShouldClose() { return glfwWindowShouldClose(MainWindow); }

	void SwapBuffers() { glfwSwapBuffers(MainWindow); }

private:
	GLFWwindow* MainWindow = nullptr;

	GLint Width, Height;
	GLint BufferWidth, BufferHeight;

};

