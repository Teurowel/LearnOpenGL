#include "ResourceManager.h"

#include <glad/glad.h>
#include <iostream>

#include "Shader.h"
#include "Texture.h"

void ResourceManager::Init()
{
	float triangleVertices[] = {
		-1.0f, -0.5f, 0.0f,	0.0f, 0.0f,
		 0.0f, 0.5f, 0.0f,	0.5f, 1.0f,
		 1.0f, -0.5f, 0.0f,	1.0f, 0.0f, 
	};

	float cubeVertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	LoadModel(EModel::triangle, triangleVertices, sizeof(triangleVertices));
	LoadModel(EModel::cube, cubeVertices, sizeof(cubeVertices));
}

void ResourceManager::LoadModel(EModel modelType, float* vertices, GLsizeiptr verticesSize)
{
	unsigned int VBO = 0;
	glGenBuffers(1, &VBO); //Generate Buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //bind buffer at GL_ARRAY_BUFFER 
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW); //set data to GL_ARRAY_BUFFER, stored data in memory on graphics card
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	VBOMap.insert(std::make_pair(modelType, VBO));
}


void ResourceManager::CreateShader(const char* shaderKey, const char* vertexPath, const char* fragmentPath)
{
	std::shared_ptr<Shader> shader = std::make_shared<Shader>();
	shader->Init(vertexPath, fragmentPath);

	shaderMap.insert(std::make_pair(shaderKey, shader));
}

void ResourceManager::CreateTexture(const char* textureKey, const char* texturePath, unsigned int sourceImageFormat, bool flipVertical)
{
	std::shared_ptr<Texture> texture = std::make_shared<Texture>();
	texture->Init(texturePath, sourceImageFormat, flipVertical);

	textureMap.insert(std::make_pair(textureKey, texture));
}

void ResourceManager::Clear()
{
	for (auto VBO : VBOMap)
	{
		glDeleteBuffers(1, &(VBO.second));
	}

	for (auto shader : shaderMap)
	{
		shader.second->Clear();
	}

	for (auto texture : textureMap)
	{
		texture.second->Clear();
	}
}

unsigned int ResourceManager::GetVBO(EModel modelEnum) const
{
	auto foundIter = VBOMap.find(modelEnum);
	if (foundIter != VBOMap.end())
	{
		return foundIter->second;
	}
	else
	{
		std::cout << "ResourceManager::GetVBO Error, not found modelEnum : " << modelEnum << std::endl;
		return 0;
	}
	
	return 0;
}

std::shared_ptr<Shader> ResourceManager::GetShader(const char* shaderKey) const
{
	auto foundIterator = shaderMap.find(shaderKey);
	if (foundIterator != shaderMap.end())
	{
		return foundIterator->second;
	}
	else
	{
		std::cout << "Shader::FindShaderProgramID Error, not found shaderKey : " << shaderKey << std::endl;
		return nullptr;
	}
}

std::shared_ptr<Texture> ResourceManager::GetTexture(const char* textureKey) const
{
	auto foundIterator = textureMap.find(textureKey);
	if (foundIterator != textureMap.end())
	{
		return foundIterator->second;
	}
	else
	{
		std::cout << "Shader::FindShaderProgramID Error, not found shaderKey : " << textureKey << std::endl;
		return nullptr;
	}
}
