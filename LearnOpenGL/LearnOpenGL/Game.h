#ifndef GAME_H
#define GAME_H

#include <memory>
#include <unordered_map>
#include <glm/glm.hpp>
#include <list>

struct GLFWwindow;
struct ModelData;

class Camera;
class ResourceManager;
class Shader;
class Object;
class Light;



class Game
{
public :
	const static float SCREEN_WIDTH;
	const static float SCREEN_HEIGHT;

public :
	Game();

	bool Init();
	void Update(float deltaTime);
	void Render();
	void Clear();

	void OnWindowResized(int width, int height);
	void OnMouseMove(double xpos, double ypos);
	void OnMouseScroll(double xOffset, double yOffset);

	const std::shared_ptr<ResourceManager> GetResourceManager();
	const std::shared_ptr<Camera> GetCamera();
	const std::shared_ptr<Light> GetLight();
	
	GLFWwindow* GetWindow();

private :
	GLFWwindow* window;

	std::shared_ptr<ResourceManager> resourceManager;
	std::shared_ptr<Camera> camera;
	std::shared_ptr<Light> light;
	

	std::unordered_map<unsigned int, std::shared_ptr<Object>> objectMap;




	bool isWireFrameMode = false;
private :
	void InitGLFW();
	bool CreateGLFWWindow();
	bool InitGLAD();
	void InitSystem();
	void InitViewport(int x, int y, int width, int height);
	void InitResourceManager();


	void CreateObject(unsigned int& objectID,
		std::shared_ptr<ModelData> modelData,
		std::shared_ptr<Shader> shader,
		std::shared_ptr<std::list<const char*>> textureKeys,
		bool hasColor, bool hasTexture,
		const glm::vec3& position,
		const glm::vec3& scale);


	void ProcessInput(float deltaTime);
	

	void ClearBuffer();

	

	void EnableWireFrameMode(bool enable);
};

#endif

