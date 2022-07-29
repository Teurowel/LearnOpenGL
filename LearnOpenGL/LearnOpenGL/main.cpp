#include <glad/glad.h>
#include <GLFW/glfw3.h>

// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>

#include "Game.h"

//void InitElementBufferObject()
//{
//	//An EBO is a buffer, just like a vertex buffer object, that stores indices that OpenGL uses to decide what vertices to draw. This so called indexed drawing
//	glGenBuffers(1, &EBO);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleIndices), rectangleIndices, GL_STATIC_DRAW);
//}

Game game;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

void GameLoop(GLFWwindow* window);

void SetCallbacks(GLFWwindow* window);
void OnWindowResized(GLFWwindow* window, int width, int height);
void OnMouseMove(GLFWwindow* window, double xpos, double ypos);
void OnMouseScroll(GLFWwindow* window, double xOffset, double yOffset);

int main()
{
	bool result = game.Init();
	if (result == false)
	{
		return -1;
	}

	//call this function on every window resize by registering it
	GLFWwindow* window = game.GetWindow();
	SetCallbacks(window);

	while (glfwWindowShouldClose(window) == false)
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		game.Update(deltaTime);
		game.Render();

		glfwSwapBuffers(window); //swap the color buffer (a large 2D buffer that contains color values for each pixel in GLFW's window) that is used to render to during this render iteration and show it as output to the screen.
		glfwPollEvents(); //checks if any events are triggered (like keyboard input or mouse movement events), updates the window state, and calls the corresponding functions (which we can register via callback methods)		
	}

	game.Clear();

	return 0;
}


void SetCallbacks(GLFWwindow* window)
{
	glfwSetFramebufferSizeCallback(window, OnWindowResized);
	glfwSetCursorPosCallback(window, OnMouseMove);
	glfwSetScrollCallback(window, OnMouseScroll);
}

void OnWindowResized(GLFWwindow* window, int width, int height)
{
	//std::cout << "OnWindowResized" << std::endl;
	game.OnWindowResized(width, height);
}

void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
	//std::cout << "OnMouseMove " << "Xpos : " << xpos << " Ypos : " << ypos << std::endl;
	game.OnMouseMove(xpos, ypos);
}

void OnMouseScroll(GLFWwindow* window, double xOffset, double yOffset)
{
	//std::cout << "OnMouseScroll" << "yOffset : " << yOffset << std::endl;
	game.OnMouseScroll(xOffset, yOffset);
}