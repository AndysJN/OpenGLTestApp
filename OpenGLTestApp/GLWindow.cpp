#include "GLWindow.h"

GLWindow::GLWindow() : GLWindow (800, 600)
{
}

GLWindow::GLWindow(GLint WindowWidth, GLint WindowHeight) : Width(WindowWidth), Height(WindowHeight)
{

}

int GLWindow::Initialise()
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

	MainWindow = glfwCreateWindow(Width, Height, "Test Window", NULL, NULL);

	if (!MainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get Buffer Size Information (Related to viewport and portion used)
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
}

GLWindow::~GLWindow()
{
	glfwDestroyWindow(MainWindow);
	glfwTerminate();
}