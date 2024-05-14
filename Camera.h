#pragma once

#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/ext.hpp"

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{
public:

	// Camera Attributes
	glm::vec3 position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	glm::vec3 getPosition() const;

	GLfloat Yaw;
	GLfloat Pitch;
	GLfloat Roll;

	GLfloat FOV = 60.0f;

	// Camera options
	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;
	GLfloat Zoom;

	// TODO: Tohle se pak musí pøesunout jinam
	GLfloat SprintFactor;

	// Constructor
	Camera(glm::vec3 position);

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();

	glm::vec3 getFront();
	glm::vec3 getRight();
	glm::vec3 getUp();
	GLfloat getYaw();
	GLfloat getPitch();

	void onKeyboardEvent(GLFWwindow* window, GLfloat deltaTime);
	void onMouseEvent(GLfloat xoffset, GLfloat yoffset, GLboolean constraintPitch);

	void toggleSprint();


private:
	void updateCameraVectors();

	bool is_sprint;
};
