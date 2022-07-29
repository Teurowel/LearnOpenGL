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

class Object
{
public :
	Object();
	~Object();

	void Init(unsigned int objectID,
	          std::shared_ptr<ModelData> modelData,
	          std::shared_ptr<Shader> shader,
	          std::shared_ptr<std::list<const char*>> textureKeys,
	          bool hasColor, bool hasTexture, Game* game);
	
	void Update();
	void Render();
	void Clear();
	
	void SetPosition(const glm::vec3& position);
	void SetScale(const glm::vec3& scale);
	
	std::shared_ptr<Shader> GetShader() const;
	
private :
	unsigned int objectID = 0;

	unsigned int VAO = 0;

	std::shared_ptr<ModelData> modelData = nullptr;
	std::shared_ptr<Shader> shader = nullptr;
	std::shared_ptr<std::list<const char*>> textureKeys = nullptr;

	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::mat4 worldMatrix;



	Game* game;

	
	
	void InitVertexAttributes(bool hasColor, bool hasTexture);
};

#endif

