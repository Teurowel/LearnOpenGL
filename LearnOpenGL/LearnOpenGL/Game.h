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
class Material;
class Model;

class Game
{
public :
	const static float SCREEN_WIDTH;
	const static float SCREEN_HEIGHT;
	const static int NR_POINT_LIGHTS;
public :
	Game();

	bool Init();
	void Update(float deltaTime);
	void Render();
	void Clear();

	void OnWindowResized(int width, int height);
	void OnMouseMove(double xpos, double ypos);
	void OnMouseScroll(double xOffset, double yOffset);

	const std::shared_ptr<ResourceManager> GetResourceManager() const;
	const std::shared_ptr<Camera> GetCamera() const;
	const std::shared_ptr<Light> GetDirectionalLight() const;
	const std::shared_ptr<Light> GetSpotLight() const;
	
	GLFWwindow* GetWindow();

private :
	GLFWwindow* window;

	std::shared_ptr<ResourceManager> resourceManager;
	std::shared_ptr<Camera> camera;
	
	std::shared_ptr<Light> directionalLight;
	std::vector<std::shared_ptr<Light>> pointLightVec;
	std::shared_ptr<Light> spotLight;

	std::unordered_map<unsigned int, std::shared_ptr<Object>> objectMap;
	std::unordered_map<std::string, std::shared_ptr<std::list<std::shared_ptr<Object>>>> shaderObjectMap;

	std::shared_ptr<Model> model;

	bool isWireFrameMode = false;
private :
	void InitGLFW();
	bool CreateGLFWWindow();
	bool InitGLAD();
	void InitSystem();
	void InitViewport(int x, int y, int width, int height);
	void InitResourceManager();
	void InitLights();
	void InitObjects();

	std::shared_ptr<Object> CreateObject(unsigned int& objectID,
										std::shared_ptr<Model> model,
										const std::string& shaderKey,
										const glm::vec3& position,
										const glm::vec3& scale);
	void AddToShaderObjectMap(const std::string& shaderKey, std::shared_ptr<Object> object);
	

	void ProcessInput(float deltaTime);
	

	void ClearBuffer();
	void RenderUnLitShader();
	void RenderLitShader();
	void RenderDepthTestingShader();
	void RenderStencilTestingShader();
	void RenderShaderObjectList(std::string key, std::shared_ptr<Shader> shader);

	void EnableWireFrameMode(bool enable);
};

#endif

