#pragma once

#include <iostream>
#include "Camera.h"
#include "glm/common.hpp"
#include "glm/ext.hpp"

Camera::Camera(glm::vec3 position)
	: Position(position)
{
	this->Position = glm::vec3(-15.0f, 2.0f, 10.0f);
	this->Front = glm::vec3(0.0f, 0.0f, -1.0f);
	this->Up = glm::vec3(0.0f, 1.0f, 0.0f);
	this->Right = glm::vec3(0.0f, 0.0f, 0.0f);
	this->WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	this->Yaw = -90.0f;
	this->Pitch = 0.0f;
	this->Roll = 0.0f;
	this->Zoom = 45.0f;

	this->MovementSpeed = 4.0f;
	this->SprintFactor = 3.0f;
	this->MouseSensitivity = 0.25f;

	this->updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::getProjectionMatrix() {
	return glm::perspective(glm::radians(FOV), 1920.0f / 1080.0f, 0.1f, 100.0f);
}

void Camera::onKeyboardEvent(GLFWwindow* window, GLfloat deltaTime)
{
	glm::vec3 direction{ 0 };

	float cameraSpeed = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? SprintFactor : 1) * MovementSpeed * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		this->Position += cameraSpeed * this->Front;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		this->Position -= cameraSpeed * this->Front;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		this->Position -= cameraSpeed * this->Right;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		this->Position += cameraSpeed * this->Right;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		this->Position += this->Up * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		this->Position -= this->Up * cameraSpeed;
	}

}

void Camera::onMouseEvent(GLfloat xoffset, GLfloat yoffset, GLboolean constraintPitch)
{
	// Redukce citlivosti pro pomalejší pohyb, mùžete upravit hodnoty 0.01f na jinou hodnotu pro úpravu citlivosti
	xoffset *= this->MouseSensitivity * 0.4f; // Snížení citlivosti x osy
	yoffset *= this->MouseSensitivity * 0.4f; // Snížení citlivosti y osy

	std::cout << "xoffset " << xoffset << std::endl;
	std::cout << "yoffset " << yoffset << std::endl;

	this->Yaw += xoffset;
	this->Pitch -= yoffset; // Odeèítáme yoffset, aby se kamera pohybovala správnì nahoru a dolù

	if (constraintPitch)
	{
		if (this->Pitch > 89.0f)
			this->Pitch = 89.0f;
		if (this->Pitch < -89.0f)
			this->Pitch = -89.0f;
	}

	this->updateCameraVectors();
}



void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
	front.y = sin(glm::radians(this->Pitch));
	front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));

	this->Front = glm::normalize(front);
	this->Right = glm::normalize(glm::cross(this->Front, glm::vec3(0.0f, 1.0f, 0.0f)));
	this->Up = glm::normalize(glm::cross(this->Right, this->Front));
}