#include "Game.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Camera.h"
#include "ResourceManager.h"
#include "Object.h"
#include "Shader.h"

const float Game::SCREEN_WIDTH = 800.0f;
const float Game::SCREEN_HEIGHT = 600.0f;

Game::Game()
	:window(nullptr)
{

}

bool Game::Init()
{
	InitGLFW();

	bool result = CreateGLFWWindow();
	if (result == false)
	{
		return result;
	}

	result = InitGLAD();
	if (result == false)
	{
		return result;
	}

	InitSystem();

	resourceManager = std::make_shared<ResourceManager>();
	resourceManager->Init();
	
	resourceManager->CreateShader("DefaultShader", "Shader/shader.vs", "Shader/shader.fs");
	
	resourceManager->CreateTexture("Container", "Texture/container.jpg", GL_RGB, false);
	resourceManager->CreateTexture("Awesomeface", "Texture/awesomeface.png", GL_RGBA, true);

	camera = std::make_shared<Camera>();


	object = std::make_shared<Object>();
	object->Init(resourceManager->GetVBO(ResourceManager::EModel::triangle), false, true);
	object->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

	return result;
}

void Game::Update(float deltaTime)
{
	ProcessInput(deltaTime);

	object->Update();
}

void Game::Render()
{
	ClearBuffer();

	std::shared_ptr<Shader> shader = resourceManager->GetShader("DefaultShader");
	shader->Use();
	shader->SetInt("texture1", 0);
	shader->SetInt("texture2", 1);
	shader->SetMatrix("view", camera->GetViewMatrix());
	shader->SetMatrix("projection", camera->GetProjMatrix());

	object->Render(resourceManager, shader);
}

void Game::Clear()
{
	object->Clear();

	resourceManager->Clear();

	glfwTerminate();
}

std::shared_ptr<Camera> Game::GetCamera()
{
	return camera;
}

GLFWwindow* Game::GetWindow()
{
	return window;
}

void Game::InitGLFW()
{
	glfwInit(); //initialize GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //what option we want to configure, https://www.glfw.org/docs/latest/window.html#window_hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	
}

bool Game::CreateGLFWWindow()
{
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW widnow" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	return true;
}

bool Game::InitGLAD()
{
	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == false)
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	return true;
}

void Game::InitSystem()
{
	InitViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glEnable(GL_DEPTH_TEST);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//wherever we move the mouse it won't be visible and it should not leave the window.
}

void Game::InitViewport(int x, int y, int width, int height)
{
	//tell OpenGL the size of the rendering window
	//processed coordinates in OpenGL are between -1 and 1 so we effectively map from the range (-1 to 1) to (0, 800) and (0, 600).
	//(-0.5,0.5) would be mapped to (200,450)
	glViewport(x, y, width, height); //location of the lower left corner, width and height of the rendering window in pixels
}

void Game::ProcessInput(float deltaTime)
{
	//if it's not pressed, glfwGetKey returns GLFW_RELEASE
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	else if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
	{
		isWireFrameMode = !isWireFrameMode;
		EnableWireFrameMode(isWireFrameMode);
	}

	camera->ProcessInput(window, deltaTime);
}

void Game::OnWindowResized(int width, int height)
{
	std::cout << "OnWindowResized" << std::endl;
	InitViewport(0, 0, width, height);
}

void Game::OnMouseMove(double xpos, double ypos)
{
	//std::cout << "OnMouseMove " << "Xpos : " << xpos << " Ypos : " << ypos << std::endl;
	camera->OnMouseMove(xpos, ypos);
}

void Game::OnMouseScroll(double xOffset, double yOffset)
{
	//std::cout << "OnMouseScroll" << "yOffset : " << yOffset << std::endl;
	camera->OnMouseScroll(yOffset);
}

void Game::EnableWireFrameMode(bool enable)
{
	if (enable == true)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void Game::ClearBuffer()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //set clear color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear Color_Buffer using clear color, clear depth buffer
}