#include "ResourceManager.h"

#include <glad/glad.h>
#include <iostream>

#include "Shader.h"
#include "ModelData.h"
#include "Material.h"

void ResourceManager::CreateModel()
{
	float triangleVertices[] = {
		-1.0f, -0.5f, 0.0f,	0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
		 0.0f, 0.5f, 0.0f,	0.5f, 1.0f, 0.0f,  0.0f, -1.0f,
		 1.0f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
	};

	float cubeVertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
	};

	LoadModel(EModel::triangle, triangleVertices, sizeof(triangleVertices), 32);
	LoadModel(EModel::cube, cubeVertices, sizeof(cubeVertices), 32);
}

void ResourceManager::LoadModel(EModel modelType, float* vertices, GLsizeiptr verticesSize, unsigned int vertexStride)
{
	std::shared_ptr<ModelData> modelData = std::make_shared<ModelData>();
	
	glGenBuffers(1, &modelData->VBO); //Generate Buffer
	glBindBuffer(GL_ARRAY_BUFFER, modelData->VBO); //bind buffer at GL_ARRAY_BUFFER 
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW); //set data to GL_ARRAY_BUFFER, stored data in memory on graphics card
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	modelData->vertexCount = verticesSize / vertexStride;
	
	modelDataMap.insert(std::make_pair(modelType, modelData));
}


void ResourceManager::CreateShader(const char* shaderKey, const char* vertexPath, const char* fragmentPath)
{
	std::shared_ptr<Shader> shader = std::make_shared<Shader>();
	shader->Init(vertexPath, fragmentPath);

	shaderMap.insert(std::make_pair(shaderKey, shader));
}

// void ResourceManager::CreateTexture(const char* textureKey, const char* texturePath, bool flipVertical)
// {
// 	std::shared_ptr<Texture> texture = std::make_shared<Texture>();
// 	texture->Init(texturePath, flipVertical);
//
// 	textureMap.insert(std::make_pair(textureKey, texture));
// }

void ResourceManager::CreateMaterial(const char* materialKey, const char* diffuseTextureKey, const char* specularTextureKey,
                                     float shininess)
{
	std::shared_ptr<Material> material = std::make_shared<Material>();
	material->Init(diffuseTextureKey, specularTextureKey, shininess);

	materialMap.insert(std::make_pair(materialKey, material));
}

void ResourceManager::Clear()
{
	for (auto modelData : modelDataMap)
	{
		glDeleteBuffers(1, &(modelData.second->VBO));
	}

	for (auto shader : shaderMap)
	{
		shader.second->Clear();
	}

	// for (auto texture : textureMap)
	// {
	// 	texture.second->Clear();
	// }

	for(auto material : materialMap)
	{
		material.second->Clear();
	}
}

const std::shared_ptr<ModelData> ResourceManager::GetModelData(EModel modelEnum) const
{
	auto foundIter = modelDataMap.find(modelEnum);
	if (foundIter != modelDataMap.end())
	{
		return foundIter->second;
	}
	else
	{
		std::cout << "ResourceManager::GetModelData Error, not found modelEnum : " << modelEnum << std::endl;
		return 0;
	}
	
	return 0;
}

const std::shared_ptr<Shader> ResourceManager::GetShader(const char* shaderKey) const
{
	return FindMapElement<std::unordered_map<const char*, std::shared_ptr<Shader>>, std::shared_ptr<Shader>>(shaderMap, shaderKey);
}

// const std::shared_ptr<Texture> ResourceManager::GetTexture(const char* textureKey) const
// {
// 	return FindMapElement<std::unordered_map<const char*, std::shared_ptr<Texture>>, std::shared_ptr<Texture>>(textureMap, textureKey);
// }

const std::shared_ptr<Material> ResourceManager::GetMaterial(const char* materialKey) const
{
	return FindMapElement<std::unordered_map<const char*, std::shared_ptr<Material>>, std::shared_ptr<Material>>(materialMap, materialKey);
}

template <class MAP_TYPE, class ELEMENT_TYPE>
const ELEMENT_TYPE ResourceManager::FindMapElement(MAP_TYPE map, const char* key) const
{
	auto foundIterator = map.find(key);
	if (foundIterator != map.end())
	{
		return foundIterator->second;
	}
	else
	{
		std::cout << "ResourceManager::FindMapElement Error, not found Key : " << key << std::endl;
		return nullptr;
	}
}