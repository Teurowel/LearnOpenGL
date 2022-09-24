#include "ResourceManager.h"

#include <iostream>

#include "Shader/Shader.h"
#include "Model.h"

// void ResourceManager::LoadModel(const std::string& path, const std::string& key)
// {
// 	// float triangleVertices[] = {
// 	// 	-1.0f, -0.5f, 0.0f,	0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
// 	// 	 0.0f, 0.5f, 0.0f,	0.5f, 1.0f, 0.0f,  0.0f, -1.0f,
// 	// 	 1.0f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
// 	// };
// 	//
// 	// float cubeVertices[] = {
// 	// -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
// 	//  0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
// 	//  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
// 	//  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
// 	// -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
// 	// -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
// 	//
// 	// -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,
// 	//  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
// 	//  0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
// 	//  0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
// 	// -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
// 	// -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,
// 	//
// 	// -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
// 	// -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
// 	// -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
// 	// -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
// 	// -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
// 	// -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
// 	//
// 	//  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
// 	//  0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
// 	//  0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
// 	//  0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
// 	//  0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
// 	//  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
// 	//
// 	// -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
// 	//  0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
// 	//  0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
// 	//  0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
// 	// -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
// 	// -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
// 	//
// 	// -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
// 	//  0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
// 	//  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
// 	//  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
// 	// -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
// 	// -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
// 	// };
// 	//
// 	// LoadModel(EModel::triangle, triangleVertices, sizeof(triangleVertices), 32);
// 	// LoadModel(EModel::cube, cubeVertices, sizeof(cubeVertices), 32);
//
// 	CheckDuplicationAndLoadModel(path, key);
// }

void ResourceManager::LoadModel(const std::string& path, const std::string& key)
{
	auto iter = modelMap.find(key);
	if(iter == modelMap.end())
	{
		modelMap.insert(std::make_pair(key, std::make_shared<Model>(path)));
	}
}

void ResourceManager::LoadShader(const Game* game, const std::string& shaderKey, const std::string& vertexPath, const std::string& fragmentPath)
{
	auto iter = shaderMap.find(shaderKey);
	if(iter == shaderMap.end())
	{
		std::shared_ptr<Shader> shader = std::make_shared<Shader>();
		shader->Init(game, vertexPath, fragmentPath);

		shaderMap.insert(std::make_pair(shaderKey, shader));	
	}
}

void ResourceManager::Clear()
{
	for (auto model : modelMap)
	{
		model.second->Clear();
	}

	for (auto shader : shaderMap)
	{
		shader.second->Clear();
	}
}

const std::shared_ptr<Model> ResourceManager::GetModel(const std::string& key) const
{
	return FindMapElement<std::unordered_map<std::string, std::shared_ptr<Model>>, std::shared_ptr<Model>>(modelMap, key);
}

const std::shared_ptr<Shader> ResourceManager::GetShader(const std::string& key) const
{
	return FindMapElement<std::unordered_map<std::string, std::shared_ptr<Shader>>, std::shared_ptr<Shader>>(shaderMap, key);
}

template <class MAP_TYPE, class ELEMENT_TYPE>
const ELEMENT_TYPE ResourceManager::FindMapElement(MAP_TYPE map, const std::string& key) const
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