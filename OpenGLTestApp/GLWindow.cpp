#include "GLWindow.h"

GLWindow::GLWindow() : GLWindow (800, 600)
{

}

GLWindow::GLWindow(GLint WindowWidth, GLint WindowHeight) : Width(WindowWidth), Height(WindowHeight)
{
	for (size_t i = 0; i < 1024; ++i)
	{
		Keys[i] = false;
	}

	XChange = 0.0f;
	YChange = 0.0f;
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

	// Handle Key + Mouse Input
	CreateCallbacks();

	// Lock Cursor to Screen
	glfwSetInputMode(MainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

	// Catch the pointer to the window to pass to the callback to select the correct window - Input
	glfwSetWindowUserPointer(MainWindow, this);
}

void GLWindow::CreateCallbacks()
{
	glfwSetKeyCallback(MainWindow, HandleKeys);
	glfwSetCursorPosCallback(MainWindow, HandleMouse);
}

GLfloat GLWindow::GetXChange()
{
	GLfloat TheChange = XChange;
	XChange = 0.0f;
	return TheChange;
}

GLfloat GLWindow::GetYChange()
{
	GLfloat TheChange = YChange;
	YChange = 0.0f;
	return TheChange;
}

void GLWindow::HandleKeys(GLFWwindow* Window, int Key, int Code, int Action, int Mode)
{
	GLWindow* TheWindow = static_cast<GLWindow*>(glfwGetWindowUserPointer(Window));

	if (Key == GLFW_KEY_ESCAPE && Action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(Window, GL_TRUE);
	}

	if (Key >= 0 && Key < 1024)
	{
		if (Action == GLFW_PRESS)
		{
			TheWindow->Keys[Key] = true;
			printf("Pressed: %d\n", Key);
		}
		else if (Action == GLFW_RELEASE)
		{
			TheWindow->Keys[Key] = false;
			printf("Released: %d\n", Key);
		}
	}
}

void GLWindow::HandleMouse(GLFWwindow* Window, double XPos, double YPos)
{
	GLWindow* TheWindow = static_cast<GLWindow*>(glfwGetWindowUserPointer(Window));

	if (TheWindow->MouseFirstMoved)
	{
		TheWindow->LastX = XPos;
		TheWindow->LastY = YPos;
		TheWindow->MouseFirstMoved = false;
	}

	TheWindow->XChange = XPos - TheWindow->LastX;
	TheWindow->YChange = TheWindow->LastY - YPos; //To avoid inversion on Mouse Movement. Can improve with a setting to flip.

	TheWindow->LastX = XPos;
	TheWindow->LastY = YPos;

	printf("x: % .6f, y: %.6f\n", TheWindow->XChange, TheWindow->YChange);
}

GLWindow::~GLWindow()
{
	glfwDestroyWindow(MainWindow);
	glfwTerminate();
}