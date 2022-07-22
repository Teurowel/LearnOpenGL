#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Source.h"

void InitGLFW()
{
	glfwInit(); //initialize GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //what option we want to configure, https://www.glfw.org/docs/latest/window.html#window_hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

bool CreateGLFWWindow(GLFWwindow** window)
{
	*window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (*window == NULL)
	{
		std::cout << "Failed to create GLFW widnow" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(*window);
	return true;
}

bool InitGLAD()
{
	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == false)
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	return true;
}

void InitViewport(int x, int y, int width, int height)
{
	//tell OpenGL the size of the rendering window
	//processed coordinates in OpenGL are between -1 and 1 so we effectively map from the range (-1 to 1) to (0, 800) and (0, 600).
	//(-0.5,0.5) would be mapped to (200,450)
	glViewport(x, y, width, height); //location of the lower left corner, width and height of the rendering window in pixels
}

void OnWindowResized(GLFWwindow* window, int width, int height)
{
	std::cout << "OnWindowResized" << std::endl;
	InitViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window)
{
	//if it's not pressed, glfwGetKey returns GLFW_RELEASE
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void RenderLoop(GLFWwindow* window)
{
	while (glfwWindowShouldClose(window) == false)
	{
		ProcessInput(window);

		//rendering commnads here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //set clear color
		glClear(GL_COLOR_BUFFER_BIT); //clear Color_Buffer using clear color


		glfwPollEvents(); //checks if any events are triggered (like keyboard input or mouse movement events), updates the window state, and calls the corresponding functions (which we can register via callback methods)
		glfwSwapBuffers(window); //swap the color buffer (a large 2D buffer that contains color values for each pixel in GLFW's window) that is used to render to during this render iteration and show it as output to the screen.
		
	}
}

void Clear()
{
	glfwTerminate();
}



int main()
{
	InitGLFW();

	GLFWwindow* window = nullptr;
	bool result = CreateGLFWWindow(&window);
	if (result == false)
	{
		return -1;
	}

	result = InitGLAD();
	if (result == false)
	{
		return -1;
	}

	InitViewport(0, 0, 800, 600);

	//call this function on every window resize by registering it
	glfwSetFramebufferSizeCallback(window, OnWindowResized);

	RenderLoop(window);

	Clear();
	return 0;
}

