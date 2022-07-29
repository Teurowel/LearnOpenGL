#ifndef GAME_H
#define GAME_H

#include <memory>
#include <unordered_map>
#include <glm/glm.hpp>

class Camera;
struct GLFWwindow;
class ResourceManager;
class Shader;
class Object;
struct ModelData;

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

	std::shared_ptr<Camera> GetCamera();
	GLFWwindow* GetWindow();



private :
	GLFWwindow* window;

	std::shared_ptr<ResourceManager> resourceManager;

	std::shared_ptr<Camera> camera;

	bool isWireFrameMode = false;

	std::unordered_map<unsigned int, std::shared_ptr<Object>> objectMap;

private :
	void InitGLFW();
	bool CreateGLFWWindow();
	bool InitGLAD();
	void InitSystem();
	void InitViewport(int x, int y, int width, int height);
	void InitResourceManager();


	void CreateObject(unsigned int& objectID, std::shared_ptr<ModelData> modelData, bool hasColor, bool hasTexture, const glm::vec3& position);


	void ProcessInput(float deltaTime);
	

	void ClearBuffer();

	

	void EnableWireFrameMode(bool enable);
};

#endif

