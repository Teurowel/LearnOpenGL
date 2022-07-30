#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
struct GLFWwindow;

class Camera
{
public:
	Camera();
	~Camera();

	void ProcessInput(GLFWwindow* window, float deltaTime);

	void OnMouseMove(double mouseXPos, double mouseYPos);
	void OnMouseScroll(double scrollYOffset);

	void SetPosition(const glm::vec3& position);
	
	const glm::mat4& GetViewMatrix();
	const glm::mat4& GetProjMatrix();
	const glm::vec3& GetPosition();
	const glm::vec3& GetCameraFront();

private :
	const float MAX_CAMERA_FOV;
	const float MIN_CAMERA_FOV;
	const float MAX_CAMERA_PITCH;
	const float MIN_CAMERA_PITCH;

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	
	float cameraSpeed = 2.5f;
	float cameraYaw = -90.0f;
	float cameraPitch = 0.0f;
	float cameraFov = 45.0f;
	bool firstMouseMove = true;

	float lastMousePosX;
	float lastMousePosY;
	float mouseSensitivity = 0.1f;

	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;

};

#endif
