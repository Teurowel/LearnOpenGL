#ifndef OBJECT_H
#define OBJECT_H

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Shader;
class ResourceManager;
struct ModelData;

class Object
{
public :
	Object();
	~Object();

	void Init(unsigned int objectID, std::shared_ptr<ModelData> modelData, bool hasColor, bool hasTexture);
	void Update();
	void Render(std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<Shader> shader);
	void Clear();

	void SetPosition(const glm::vec3& position);

private :
	unsigned int objectID = 0;

	unsigned int VAO = 0;

	std::shared_ptr<ModelData> modelData = nullptr;

	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::mat4 worldMatrix;

	void InitVertexAttributes(bool hasColor, bool hasTexture);
};

#endif

