#include "Game.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Camera.h"
#include "Object.h"
#include "Resources/Shader/Shader.h"
#include "Resources/ResourceManager.h"
#include "Light.h"
#include "Resources/Model.h"

const float Game::SCREEN_WIDTH = 800.0f;
const float Game::SCREEN_HEIGHT = 600.0f;
const int Game::NR_POINT_LIGHTS = 3;

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
	camera->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));

	InitLights();
	
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
	
	//	//point lights set
	//	std::string attributeName = "pointLights[";
	//	for(int i = 0; i < NR_POINT_LIGHTS; ++i)
	//	{
	//		attributeName = attributeName + std::to_string(i) + "].";
	//		
	//		shader->SetVec3(attributeName + "position", pointLightVec[i]->GetPosition());
	//		shader->SetVec3(attributeName + "ambient", pointLightVec[i]->GetAmbientColor());
 //           shader->SetVec3(attributeName + "diffuse", pointLightVec[i]->GetDiffuseColor());
 //           shader->SetVec3(attributeName + "specular", pointLightVec[i]->GetSpecularColor());
 //           shader->SetFloat(attributeName + "constant", pointLightVec[i]->GetCosntant());
 //           shader->SetFloat(attributeName + "linear", pointLightVec[i]->GetLinear());
 //           shader->SetFloat(attributeName + "quadratic", pointLightVec[i]->GetQuadratic());
	//		
	//		attributeName = "pointLights[";
	//	}
	//	


	// std::shared_ptr<Shader> shader = nullptr;
	// for(auto shaderObjectElement : shaderObjectMap)
	// {
	// 	shader = resourceManager->GetShader(shaderObjectElement.first);
	//
	// 	shader->Use();
	//
	// 	auto iter = shaderObjectElement.second->begin();
	// 	auto iterEnd = shaderObjectElement.second->end();
	//
	// 	for(; iter != iterEnd; ++iter)
	// 	{
	// 		(*iter)->Render(shader);
	// 	}
	// 	
	// 	shader->UnUse();
	// }

	RenderUnLitShader();
	
	RenderLitShader();
	RenderStencilTestingShader();
	
	RenderDepthTestingShader();
}

void Game::RenderUnLitShader()
{
	glStencilMask(0x00); //Don't update stencil buffer
	
	std::shared_ptr<Shader> shader = resourceManager->GetShader("UnLitShader");

	shader->Use();

	RenderShaderObjectList("UnLitShader", shader);
		
	shader->UnUse();

	glStencilMask(0xFF);
}

void Game::RenderLitShader()
{
	//Writing to the stencil buffer
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF); 
	
	std::shared_ptr<Shader> shader = resourceManager->GetShader("LitShader");

	shader->Use();

	RenderShaderObjectList("LitShader", shader);
		
	shader->UnUse();

	glStencilMask(0x00);
}

void Game::RenderDepthTestingShader()
{
	glStencilMask(0x00); //Don't update stencil buffer
	
	std::shared_ptr<Shader> shader = resourceManager->GetShader("DepthTestingShader");

	shader->Use();

	RenderShaderObjectList("DepthTestingShader", shader);
		
	shader->UnUse();

	glStencilMask(0xFF);
}

void Game::RenderStencilTestingShader()
{
	//disable stencil writing, and compare to stencil buffer, only draw area which doesn't have 1
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST); //turn off depth test for outline
	
	
	std::shared_ptr<Shader> shader = resourceManager->GetShader("StencilTestingShader");

	shader->Use();

	RenderShaderObjectList("StencilTestingShader", shader);
		
	shader->UnUse();

	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glEnable(GL_DEPTH_TEST);
}



void Game::RenderShaderObjectList(std::string key, std::shared_ptr<Shader> shader)
{
	auto shaderObjectList = shaderObjectMap[key];
	auto iter = shaderObjectList->begin();
	auto iterEnd = shaderObjectList->end();

	for(; iter != iterEnd; ++iter)
	{
		(*iter)->Render(shader);
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
	glDepthFunc(GL_LESS);

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	
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
	resourceManager->LoadModel("Asset/Objects/Backpack/backpack.obj", "Backpack");
	
	resourceManager->LoadShader(this,
		"UnLitShader", "Asset/Shaders/UnLitVertexShader.vs", "Asset/Shaders/UnLitFragmentShader.fs",
		ResourceManager::UnLit);
	resourceManager->LoadShader(this,
		"LitShader", "Asset/Shaders/LitVertexShader.vs", "Asset/Shaders/LitFragmentShader.fs",
		ResourceManager::Lit);
	resourceManager->LoadShader(this,
		"DepthTestingShader", "Asset/Shaders/DepthTestingVertexShader.vs", "Asset/Shaders/DepthTestingFragmentShader.fs",
		ResourceManager::DepthTesting);
	resourceManager->LoadShader(this,
		"StencilTestingShader", "Asset/Shaders/StencilTestingVertexShader.vs", "Asset/Shaders/StencilTestingFragmentShader.fs",
		ResourceManager::StencilTesting);
}

void Game::InitLights()
{
	directionalLight = std::make_shared<Light>();
	directionalLight->Init(glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(-0.2f, -1.0f, -0.3f),
				glm::vec3(0.2f, 0.2f, 0.2f),
				glm::vec3(1.0f, 1.0f, 1.0f),
				glm::vec3(1.0f, 1.0f, 1.0f),
				0.0f, 0.0f, 0.0f);

	
	/*glm::vec3 pointLightColors[] ={
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
	};
	
	glm::vec3 pointLightPositions[] = {
		glm::vec3( 0.0f,  1.5f,  -2.5f),
		glm::vec3( 2.5f, 1.5f, 2.5f),
		glm::vec3(-2.5f,  1.5f, 2.5f),
	};

	pointLightVec.reserve(NR_POINT_LIGHTS);
	for(int i = 0; i < NR_POINT_LIGHTS; ++i)
	{
		std::shared_ptr<Light> pointLight = std::make_shared<Light>();
		pointLight->Init(pointLightPositions[i],
			glm::vec3(0.0f, 0.0f, 0.0f),
			pointLightColors[i] * 0.2f,
				pointLightColors[i],
				pointLightColors[i],
				1.0f, 0.09f, 0.032f);

		pointLightVec.push_back(pointLight);
	}
*/


	spotLight = std::make_shared<Light>();
	spotLight->Init(glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.2f, 0.2f, 0.2f),
				glm::vec3(1.0f, 1.0f, 1.0f),
				glm::vec3(1.0f, 1.0f, 1.0f),
				1.0f, 0.09f, 0.032f);
}

void Game::InitObjects()
{
	unsigned int objectID = 0;

	// for(int i =0 ; i < NR_POINT_LIGHTS; ++i)
	// {
	// 	std::shared_ptr<Object> pointLight = CreateObject(objectID,
	// 			resourceManager->GetModelData(ResourceManager::EModel::cube),
	// 			"UnLitShader",
	// 			nullptr,
	// 			false, true, true,
	// 			pointLightVec[i]->GetPosition(),
	// 			glm::vec3(0.1f, 0.1f, 0.1f));
	//
	// 	pointLight->SetUnLitColor(pointLightVec[i]->GetDiffuseColor());
	// }
	

	std::shared_ptr<Object> unLitObj = CreateObject(objectID,
		resourceManager->GetModel("Backpack"),
		"UnLitShader",
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f));
	unLitObj->SetUnLitColor(glm::vec3(1.0f, 0.0f, 0.0f));

	unLitObj = CreateObject(objectID,
		resourceManager->GetModel("Backpack"),
		"UnLitShader",
		glm::vec3(0.0f, 0.0f, -4.0f),
		glm::vec3(1.0f, 1.0f, 1.0f));
	unLitObj->SetUnLitColor(glm::vec3(0.0f, 1.0f, 0.0f));
	


	
	CreateObject(objectID,
		resourceManager->GetModel("Backpack"),
		"LitShader",
		glm::vec3(4.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f));

	CreateObject(objectID,
		resourceManager->GetModel("Backpack"),
		"LitShader",
		glm::vec3(4.0f, 0.0f, -4.0f),
		glm::vec3(1.0f, 1.0f, 1.0f));

	float scale = 1.025f;
	CreateObject(objectID,
		resourceManager->GetModel("Backpack"),
		"StencilTestingShader",
		glm::vec3(4.0f, 0.0f, 0.0f),
		glm::vec3(scale, scale, scale));

	CreateObject(objectID,
		resourceManager->GetModel("Backpack"),
		"StencilTestingShader",
		glm::vec3(4.0f, 0.0f, -4.0f),
		glm::vec3(scale, scale, scale));


	
	
	CreateObject(objectID,
		resourceManager->GetModel("Backpack"),
		"DepthTestingShader",
		glm::vec3(8.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f));

	CreateObject(objectID,
		resourceManager->GetModel("Backpack"),
		"DepthTestingShader",
		glm::vec3(8.0f, 0.0f, -4.0f),
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

const std::shared_ptr<ResourceManager> Game::GetResourceManager() const
{
	return resourceManager;
}

const std::shared_ptr<Camera> Game::GetCamera() const
{
	return camera;
}

const std::shared_ptr<Light> Game::GetDirectionalLight() const
{
	return directionalLight;
}

const std::shared_ptr<Light> Game::GetSpotLight() const
{
	return spotLight;
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); //clear Color_Buffer using clear color, clear depth buffer, clear stencil buffer
}

std::shared_ptr<Object> Game::CreateObject(unsigned int& objectID,
                        std::shared_ptr<Model> model,
                        const std::string& shaderKey,
                        const glm::vec3& position,
                        const glm::vec3& scale)
{
	std::shared_ptr<Object> object = std::make_shared<Object>();
	object->Init(objectID, model);
	object->SetPosition(position);
	object->SetScale(scale);
	
	objectMap.insert(std::make_pair(objectID, object));

	AddToShaderObjectMap(shaderKey, object);
	
	++objectID;

	return object;
}

void Game::AddToShaderObjectMap(const std::string& shaderKey, std::shared_ptr<Object> object)
{
	auto it = shaderObjectMap.find(shaderKey);
	if(it != shaderObjectMap.end())
	{
		it->second->push_back(object);
	}
	else
	{
		shaderObjectMap.insert(std::make_pair(shaderKey, std::make_shared<std::list<std::shared_ptr<Object>>>()));
		shaderObjectMap[shaderKey]->push_back(object);
	}
}