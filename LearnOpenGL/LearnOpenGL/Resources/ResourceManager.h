#ifndef RESOUCEMANAGER_H
#define RESOUCEMANAGER_H

#include <unordered_map>
#include <memory>
#include <glad/glad.h>

class Shader;
class Texture;
struct ModelData;

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
	void Clear();

	const std::shared_ptr<ModelData> GetModelData(EModel modelEnum) const;
	const std::shared_ptr<Shader> GetShader(const char* shaderKey) const;
	const std::shared_ptr<Texture> GetTexture(const char* textureKey) const;

private :
	std::unordered_map<EModel, std::shared_ptr<ModelData>> modelDataMap;
	std::unordered_map<const char*, std::shared_ptr<Shader>> shaderMap;
	std::unordered_map<const char*, std::shared_ptr<Texture>> textureMap;

	void LoadModel(EModel modelType, float* vertices, GLsizeiptr verticesSize, unsigned int vertexStride);
};

#endif

