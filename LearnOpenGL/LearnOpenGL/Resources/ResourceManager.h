#ifndef RESOUCEMANAGER_H
#define RESOUCEMANAGER_H

#include <unordered_map>
#include <memory>
#include <windows.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

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

	void Init();
	void CreateShader(const char* shaderKey, const char* vertexPath, const char* fragmentPath);
	void CreateTexture(const char* textureKey, const char* texturePath, unsigned int sourceImageFormat, bool flipVertical);
	void CreateMaterial(const char* materialKey, const char* diffuseTextureKey, const char* specularTextureKey, float shininess);
	void Clear();

	const std::shared_ptr<ModelData> GetModelData(EModel modelEnum) const;
	const std::shared_ptr<Shader> GetShader(const char* shaderKey) const;
	const std::shared_ptr<Texture> GetTexture(const char* textureKey) const;
	const std::shared_ptr<Material> GetMaterial(const char* materialKey) const;

private :
	std::unordered_map<EModel, std::shared_ptr<ModelData>> modelDataMap;
	std::unordered_map<const char*, std::shared_ptr<Shader>> shaderMap;
	std::unordered_map<const char*, std::shared_ptr<Texture>> textureMap;
	std::unordered_map<const char*, std::shared_ptr<Material>> materialMap;

	void LoadModel(EModel modelType, float* vertices, GLsizeiptr verticesSize, unsigned int vertexStride);


	template<class MAP_TYPE, class ELEMENT_TYPE>
	const ELEMENT_TYPE FindMapElement(MAP_TYPE map, const char* key) const;
};

#endif

