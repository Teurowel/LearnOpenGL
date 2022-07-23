#ifndef GAME_H
#define GAME_H

#include <memory>
class Camera;
struct GLFWwindow;

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

	std::shared_ptr<Camera> camera;

	bool isWireFrameMode = false;

private :
	void InitGLFW();
	bool CreateGLFWWindow();
	bool InitGLAD();
	void InitSystem();
	void InitViewport(int x, int y, int width, int height);
	void ProcessInput(float deltaTime);
	void EnableWireFrameMode(bool enable);
	void ClearBuffer();
};

#endif

