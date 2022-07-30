#include "Game.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Camera.h"
#include "Object.h"
#include "Resources/Shader.h"
#include "Resources/ResourceManager.h"
#include "Light.h"

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

	InitResourceManager();

	camera = std::make_shared<Camera>();
	camera->SetPosition(glm::vec3(1.5f, 2.0f, 8.0f));
	
	light = std::make_shared<Light>();
	light->Init(glm::vec3(4.0f, 2.0f, 2.0f),
	            glm::vec3(-0.2f, -1.0f, -0.3f),
	            glm::vec3(0.2f, 0.2f, 0.2f),
	            glm::vec3(0.5f, 0.5f, 0.5f),
	            glm::vec3(1.0f, 1.0f, 1.0f),
	            1.0f, 0.09f, 0.032f);
	
	InitObjects();
	
	
	return result;
}

void Game::Update(float deltaTime)
{
	ProcessInput(deltaTime);

	for (auto object : objectMap)
	{
		object.second->Update();
	}
}

void Game::Render()
{
	ClearBuffer();

	for (auto object : objectMap)
	{		
		object.second->Render();
	}
}

void Game::Clear()
{
	for (auto object : objectMap)
	{
		object.second->Clear();
	}

	resourceManager->Clear();

	glfwTerminate();
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
	window = glfwCreateWindow((int)SCREEN_WIDTH, (int)SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
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
	InitViewport(0, 0, (int)SCREEN_WIDTH, (int)SCREEN_HEIGHT);

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

void Game::InitResourceManager()
{
	resourceManager = std::make_shared<ResourceManager>();
	resourceManager->Init();

	
	resourceManager->CreateShader("UnLitShader", "Shader/UnLitVertexShader.vs", "Shader/UnLitFragmentShader.fs");
	resourceManager->CreateShader("LitShader", "Shader/LitVertexShader.vs", "Shader/LitFragmentShader.fs");

	
	resourceManager->CreateTexture("Container", "Texture/container.jpg", false);
	resourceManager->CreateTexture("Awesomeface", "Texture/awesomeface.png", true);
	resourceManager->CreateTexture("Container2", "Texture/container2.png", true);
	resourceManager->CreateTexture("Container2_Specular", "Texture/container2_specular.png", true);

	
	resourceManager->CreateMaterial("Container2", "Container2", "Container2_Specular", 32.0f);
}

void Game::InitObjects()
{
	unsigned int objectID = 0;

	CreateObject(objectID,
			resourceManager->GetModelData(ResourceManager::EModel::cube),
			resourceManager->GetShader("UnLitShader"),
			nullptr,
			false, true, true,
			light->GetLightPosition(),
			glm::vec3(0.1f, 0.1f, 0.1f));
	
	CreateObject(objectID,
		resourceManager->GetModelData(ResourceManager::EModel::cube),
		resourceManager->GetShader("LitShader"),
		resourceManager->GetMaterial("Container2"),
		false, true, true,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f));

	CreateObject(objectID,
		resourceManager->GetModelData(ResourceManager::EModel::cube),
		resourceManager->GetShader("LitShader"),
		resourceManager->GetMaterial("Container2"),
		false, true, true,
		glm::vec3(3.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f));

	CreateObject(objectID,
		resourceManager->GetModelData(ResourceManager::EModel::cube),
		resourceManager->GetShader("LitShader"),
		resourceManager->GetMaterial("Container2"),
		false, true, true,
		glm::vec3(0.0f, 0.0f, 4.0f),
		glm::vec3(1.0f, 1.0f, 1.0f));

	CreateObject(objectID,
		resourceManager->GetModelData(ResourceManager::EModel::cube),
		resourceManager->GetShader("LitShader"),
		resourceManager->GetMaterial("Container2"),
		false, true, true,
		glm::vec3(3.0f, 0.0f, 4.0f),
		glm::vec3(1.0f, 1.0f, 1.0f));
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

const std::shared_ptr<ResourceManager> Game::GetResourceManager()
{
	return resourceManager;
}

const std::shared_ptr<Camera> Game::GetCamera()
{
	return camera;
}

const std::shared_ptr<Light> Game::GetLight()
{
	return light;
}

GLFWwindow* Game::GetWindow()
{
	return window;
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

void Game::CreateObject(unsigned int& objectID,
	std::shared_ptr<ModelData> modelData,
	std::shared_ptr<Shader> shader,
	std::shared_ptr<Material> material,
	bool hasColor, bool hasTexture, bool hasNormalVector,
	const glm::vec3& position,
	const glm::vec3& scale)
{
	std::shared_ptr<Object> object = std::make_shared<Object>();
	object->Init(objectID, modelData, shader, material, hasColor, hasTexture, hasNormalVector, this);
	object->SetPosition(position);
	object->SetScale(scale);
	
	objectMap.insert(std::make_pair(objectID, object));

	++objectID;
}
