#ifndef RESOUCEMANAGER_H
#define RESOUCEMANAGER_H

#include <unordered_map>
#include <memory>
#include <windows.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Game;
class Model;
struct ModelData;

class Shader;
class Texture;
class Material;


class ResourceManager
{
public :
	enum EModel
	{
		triangle,
		rectangle,
		cube
	};

	enum eShaderType
	{
		None,
		UnLit,
		Lit,
		DepthTesting
	};

	void LoadModel(const std::string& path, const std::string& key);
	void LoadShader(const Game* game,
		const std::string& shaderKey, const std::string& vertexPath, const std::string& fragmentPath,
		eShaderType shaderType);
	void Clear();

	const std::shared_ptr<Model> GetModel(const std::string& key) const;
	const std::shared_ptr<Shader> GetShader(const std::string& key) const;


private :
	std::unordered_map<std::string, std::shared_ptr<Model>> modelMap;
	std::unordered_map<std::string, std::shared_ptr<Shader>> shaderMap;
	

	template<class MAP_TYPE, class ELEMENT_TYPE>
	const ELEMENT_TYPE FindMapElement(MAP_TYPE map, const std::string& key) const;
};

#endif

