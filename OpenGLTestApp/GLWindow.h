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

	inline GLfloat GetBufferWidth() const { return BufferWidth; }
	inline GLfloat GetBufferHeight() const { return BufferHeight; }

	bool GetShouldClose() { return glfwWindowShouldClose(MainWindow); }

	const bool* GetKeys() const { return Keys; } //Can Improve to avoid passing the entire Array

	GLfloat GetXChange();
	GLfloat GetYChange();

	void SwapBuffers() { glfwSwapBuffers(MainWindow); }

	~GLWindow();

private:
	GLFWwindow* MainWindow = nullptr;

	GLint Width, Height;
	GLint BufferWidth, BufferHeight;

	bool Keys[1024]; //Cubrir los caracteres para tener un booleano por tecla
	GLfloat LastX;
	GLfloat LastY;
	GLfloat XChange;
	GLfloat YChange;
	bool MouseFirstMoved;

	void CreateCallbacks();
	static void HandleKeys(GLFWwindow* Window, int Key, int Code, int Action, int Mode); //Match signature of the callback. (static)
	static void HandleMouse(GLFWwindow* Window, double XPos, double YPos);

};

