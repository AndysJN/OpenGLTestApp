#include "Camera.h"

MyCamera::MyCamera() : MyCamera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f)
{
}

MyCamera::MyCamera(glm::vec3 StartPosition, glm::vec3 StartWorldUp, GLfloat StartYaw, GLfloat StartPitch, GLfloat StartMovementSpeed, GLfloat StartTurnSpeed) :
	Position (StartPosition), WorldUp (StartWorldUp), Yaw (StartYaw), Pitch (StartPitch), Front (glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed (StartMovementSpeed), TurnSpeed (StartMovementSpeed)
{
	Update();
}

void MyCamera::KeyControl(const bool* Keys, GLfloat DeltaTime)
{
	GLfloat Velocity = MovementSpeed * DeltaTime;

	if(Keys[GLFW_KEY_W])
	{
		Position += Front * Velocity;
	}
	if (Keys[GLFW_KEY_S])
	{
		Position -= Front * Velocity;
	}
	if (Keys[GLFW_KEY_A])
	{
		Position -= Right * Velocity;
	}
	if (Keys[GLFW_KEY_D])
	{
		Position += Right * Velocity;
	}
}

void MyCamera::MouseControl(GLfloat XChange, GLfloat YChange)
{
	XChange *= TurnSpeed;
	YChange *= TurnSpeed;

	Yaw += XChange;
	Pitch += YChange;

	if (Pitch > 89.0f)
	{
		Pitch = 89.0f;
	}

	if (Pitch < -89.0f)
	{
		Pitch = -89.0f;
	}

	Update();
}

glm::mat4 MyCamera::CalculateViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

void MyCamera::Update()
{
	Front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front.y = sin(glm::radians(Pitch));
	Front.z = sin(glm::radians(Yaw));
	Front = glm::normalize(Front);

	Right = glm::normalize(glm::cross(Front, WorldUp)); //Puedo hacer esto porque no estoy implementando Roll, sino modificarlo.
	Up = glm::normalize(glm::cross(Right, Front));
}


MyCamera::~MyCamera()
{
}