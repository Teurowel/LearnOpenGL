#ifndef RESOUCEMANAGER_H
#define RESOUCEMANAGER_H

#include <unordered_map>
#include <memory>
#include <glad/glad.h>

class Shader;
class Texture;

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

	unsigned int GetVBO(EModel modelEnum) const;
	std::shared_ptr<Shader> GetShader(const char* shaderKey) const;
	std::shared_ptr<Texture> GetTexture(const char* textureKey) const;

private :
	std::unordered_map<EModel, unsigned int> VBOMap;
	std::unordered_map<const char*, std::shared_ptr<Shader>> shaderMap;
	std::unordered_map<const char*, std::shared_ptr<Texture>> textureMap;

	void LoadModel(EModel modelType, float* vertices, GLsizeiptr verticesSize);
};

#endif

