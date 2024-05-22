#pragma once

#include <iostream>
#include "glm/common.hpp"
#include "glm/ext.hpp"

#include "Camera.h"

Camera::Camera(glm::vec3 _position)
	: position(_position)
{
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->Front = glm::normalize(glm::vec3(0.0f) - this->position);
	this->Up = glm::vec3(0.0f, 1.0f, 0.0f);
	this->Right = glm::normalize(glm::cross(this->Front, this->Up));
	//this->WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	// horizontální otáèení
	this->Yaw = 0.0f;
	// vertikální otáèení
	//this->Pitch = 90.0f;
	this->Pitch = 0.0f;
	this->Roll = 0.0f;
	this->Zoom = 45.0f;

	this->MovementSpeed = 0.05f;
	this->SprintFactor = 3.0f;
	this->MouseSensitivity = 0.25f;

	this->updateCameraVectors();

	is_sprint = false;
}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(position, position + Front, Up);
}

glm::mat4 Camera::getProjectionMatrix() {
	return glm::perspective(glm::radians(FOV), 1920.0f / 1080.0f, 0.1f, 1000.0f);
}

void Camera::toggleSprint()
{
	is_sprint = !is_sprint;
}

void Camera::onKeyboardEvent(GLFWwindow* window, GLfloat deltaTime)
{
	//float cameraSpeed = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? SprintFactor : 1) * MovementSpeed * deltaTime;
	float cameraSpeed = (is_sprint) ? SprintFactor * MovementSpeed : MovementSpeed;
	cameraSpeed = (deltaTime < 0.002) ? 0.017 * cameraSpeed : cameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		this->position += cameraSpeed * this->Front;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		this->position -= cameraSpeed * this->Front;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		this->position -= cameraSpeed * this->Right;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		this->position += cameraSpeed * this->Right;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		this->position += cameraSpeed * this->Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		this->position -= cameraSpeed * this->Up;
	}

}

void Camera::onMouseEvent(GLfloat xoffset, GLfloat yoffset, GLboolean constraintPitch)
{
	// Redukce citlivosti pro pomalejší pohyb
	xoffset *= this->MouseSensitivity * 0.3f; // Snížení citlivosti x osy
	yoffset *= this->MouseSensitivity * 0.3f; // Snížení citlivosti y osy

	//std::cout << "xoffset " << xoffset << std::endl;
	//std::cout << "yoffset " << yoffset << std::endl;

	this->Yaw += xoffset;
	this->Pitch -= yoffset; // Odeèítáme yoffset, aby se kamera pohybovala správnì nahoru a dolù
	// invertování pohybu myši ?

	if (constraintPitch)
	{
		if (this->Pitch > 89.0f)
			this->Pitch = 89.0f;
		if (this->Pitch < -89.0f)
			this->Pitch = -89.0f;
	}

	this->updateCameraVectors();
}

glm::vec3 Camera::getPosition() const {
	return position;
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

glm::vec3 Camera::getFront() {
	return Front;
}
glm::vec3 Camera::getRight() {
	return Right;
}
glm::vec3 Camera::getUp() {
	return Up;
}

GLfloat Camera::getYaw() {
	return Yaw;
}
GLfloat Camera::getPitch() {
	return Pitch;
}

bool Camera::getSprint() {
	return is_sprint;
}
