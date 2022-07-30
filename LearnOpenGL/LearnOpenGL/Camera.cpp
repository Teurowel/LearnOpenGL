#include "Camera.h"
#include "Camera.h"
#include "Camera.h"
#include "Camera.h"
#include "Camera.h"
#include "Game.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera()
	:MAX_CAMERA_FOV(45.0f),
	MIN_CAMERA_FOV(1.0f),
	MAX_CAMERA_PITCH(89.0f),
	MIN_CAMERA_PITCH(-89.0f)
{
	//std::cout << "Camera()" << std::endl;
	lastMousePosX = Game::SCREEN_WIDTH * 0.5f;
	lastMousePosY = Game::SCREEN_HEIGHT * 0.5f;

	
}

Camera::~Camera()
{
	//std::cout << "~Camera()" << std::endl;
}

void Camera::ProcessInput(GLFWwindow* window, float deltaTime)
{
	if ((glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS))
	{
		cameraPos += cameraFront * cameraSpeed * deltaTime;
	}
	else if ((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS))
	{
		cameraPos -= cameraFront * cameraSpeed * deltaTime;
	}
	else if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS))
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;
	}
	else if ((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS))
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;
	}
}


void Camera::OnMouseMove(double mouseXPos, double mouseYPos)
{
	if (firstMouseMove)
	{
		lastMousePosX = (float)mouseXPos;
		lastMousePosY = (float)mouseYPos;
		firstMouseMove = false;
	}

	float xOffset = (float)mouseXPos - lastMousePosX;
	float yOffset = lastMousePosY - (float)mouseYPos;

	lastMousePosX = (float)mouseXPos;
	lastMousePosY = (float)mouseYPos;

	xOffset *= mouseSensitivity;
	yOffset *= mouseSensitivity;

	cameraYaw += xOffset;
	cameraPitch += yOffset;

	if (cameraPitch > MAX_CAMERA_PITCH)
	{
		cameraPitch = MAX_CAMERA_PITCH;
	}
	else if (cameraPitch < MIN_CAMERA_PITCH)
	{
		cameraPitch = MIN_CAMERA_PITCH;
	}

	glm::vec3 camDirection;
	camDirection.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
	camDirection.y = sin(glm::radians(cameraPitch));
	camDirection.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
	cameraFront = glm::normalize(camDirection);
}

void Camera::OnMouseScroll(double scrollYOffset)
{
	cameraFov -= (float)scrollYOffset;

	if (cameraFov < MIN_CAMERA_FOV)
	{
		cameraFov = MIN_CAMERA_FOV;
	}
	else if (cameraFov > MAX_CAMERA_FOV)
	{
		cameraFov = MAX_CAMERA_FOV;
	}
}

void Camera::SetPosition(const glm::vec3& position)
{
	cameraPos = position;
}

const glm::mat4& Camera::GetViewMatrix()
{
	viewMatrix = glm::mat4(1.0f);
	viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	return viewMatrix;
}

const glm::mat4& Camera::GetProjMatrix()
{
	projMatrix = glm::mat4(1.0f);
	projMatrix = glm::perspective(glm::radians(cameraFov), Game::SCREEN_WIDTH / Game::SCREEN_HEIGHT, 0.1f, 100.0f);

	return projMatrix;
}

const glm::vec3& Camera::GetPosition()
{
	return cameraPos;
}

const glm::vec3& Camera::GetCameraFront()
{
	return cameraFront;
}
