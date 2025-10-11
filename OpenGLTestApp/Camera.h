#pragma once

#include <GL\glew.h>

#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>

#include <GLFW/glfw3.h>

class MyCamera
{
public:

	MyCamera();
	MyCamera(glm::vec3 StartPosition, glm::vec3 StartWorldUp, GLfloat StartYaw, GLfloat StartPitch, GLfloat StartMovementSpeed, GLfloat StartTurnSpeed);

	void KeyControl(const bool* Keys);

	glm::mat4 CalculateViewMatrix();

	~MyCamera();


private:

	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	GLfloat Yaw;
	GLfloat Pitch;
	GLfloat Roll;

	GLfloat MovementSpeed;
	GLfloat TurnSpeed;

	void Update();
};

