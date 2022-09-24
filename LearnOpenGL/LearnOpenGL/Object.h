#ifndef OBJECT_H
#define OBJECT_H

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <list>

struct ModelData;

class Shader;
class ResourceManager;
class Camera;
class Game;
class Material;
class Model;

class Object
{
public :
	Object();
	~Object();

	void Init(unsigned int objectID, std::shared_ptr<Model> model);

	void Update();
	void Render(std::shared_ptr<Shader> shader);
	void Clear();
	
	void SetPosition(const glm::vec3& position);
	void SetScale(const glm::vec3& scale);
	void SetUnLitColor(const glm::vec3& color);
	
	std::shared_ptr<Shader> GetShader() const;
	
private :
	unsigned int objectID = 0;

	std::shared_ptr<Model> model = nullptr;

	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::mat4 worldMatrix;

	glm::vec3 unLitColor = glm::vec3(0.0f, 0.0f, 0.0f);
};

#endif

