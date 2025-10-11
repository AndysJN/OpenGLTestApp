#include "Camera.h"

MyCamera::MyCamera() : MyCamera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.01f)
{
}

MyCamera::MyCamera(glm::vec3 StartPosition, glm::vec3 StartWorldUp, GLfloat StartYaw, GLfloat StartPitch, GLfloat StartMovementSpeed, GLfloat StartTurnSpeed) :
	Position (StartPosition), WorldUp (StartWorldUp), Yaw (StartYaw), Pitch (StartPitch), Front (glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed (StartMovementSpeed), TurnSpeed (StartMovementSpeed)
{
	Update();
}

void MyCamera::KeyControl(const bool* Keys)
{
	if(Keys[GLFW_KEY_W])
	{
		Position += Front * MovementSpeed;
	}
	if (Keys[GLFW_KEY_S])
	{
		Position -= Front * MovementSpeed;
	}
	if (Keys[GLFW_KEY_A])
	{
		Position -= Right * MovementSpeed;
	}
	if (Keys[GLFW_KEY_D])
	{
		Position += Right * MovementSpeed;
	}
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