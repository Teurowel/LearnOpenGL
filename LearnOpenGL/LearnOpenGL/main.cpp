#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Game.h"
#include "Camera.h"


unsigned int vertexShaderID = 0;
unsigned int vertexShaderID2 = 0;

unsigned int fragmentShaderID = 0;
unsigned int fragmentShaderID2 = 0;

unsigned int triangleVAO = 0;
unsigned int triangle2VAO = 0;

unsigned int rectangleVAO = 0;

unsigned int cubeVAO = 0;

unsigned int VBO = 0;

unsigned int EBO = 0;

unsigned int texture1 = 0;
unsigned int texture2 = 0;



float triangleVertices[] = {
	 -1.0f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,
	 0.0f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,
	 -0.5f,  0.5f, 0.0f,	0.0f, 0.0f, 1.0f
};

float triangleTexCoords[] = {
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.5f, 1.0f
};

float triangleVertices2[] = {
	 0.0f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,
	 1.0f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f
};

float rectangleVertices[] = {
	// positions          // colors           // texture coords
	0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
	0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};
unsigned int rectangleIndices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};

float cubeVertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

void Clear()
{
	glDeleteVertexArrays(1, &rectangleVAO);
	glDeleteVertexArrays(1, &triangleVAO);
	glDeleteVertexArrays(1, &triangle2VAO);

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void InitVertexArrayObject(unsigned int& VAO)
{
	//stores our vertex attribute configuration and which VBO to use
	//when you have multiple objects you want to draw, you first generate/configure all the VAOs (and thus the required VBO and attribute pointers)
	//and store those for later use. The moment we want to draw one of our objects, we take the corresponding VAO, bind it, then draw the object and unbind the VAO again.
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
}

void InitVertexBuffer(int verticesSize, const void* vertices)
{
	glGenBuffers(1, &VBO); //Generate Buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //bind buffer at GL_ARRAY_BUFFER 
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW); //set data to GL_ARRAY_BUFFER, stored data in memory on graphics card
}

void InitVertexAttributes()
{
	//Each vertex attribute takes its data from memory managed by a VBO and which VBO it takes its data from (you can have multiple VBOs) is determined by the VBO currently bound to GL_ARRAY_BUFFER when calling glVertexAttribPointer
	//we specified how OpenGL should interpret the vertex data

	////Position attribute
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); //location in Vertexshader, size of vertex attribute(vec3), normalize data, stride(space between data), offset where position data begin
	//glEnableVertexAttribArray(0);
	////Color
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);



	////Position attribute
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //location in Vertexshader, size of vertex attribute(vec3), normalize data, stride(space between data), offset where position data begin
	//glEnableVertexAttribArray(0);
	////Color
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	////Texture
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	//glEnableVertexAttribArray(2);

	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); //location in Vertexshader, size of vertex attribute(vec3), normalize data, stride(space between data), offset where position data begin
	glEnableVertexAttribArray(0);

	//Texture
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

}

void InitElementBufferObject()
{
	//An EBO is a buffer, just like a vertex buffer object, that stores indices that OpenGL uses to decide what vertices to draw. This so called indexed drawing
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleIndices), rectangleIndices, GL_STATIC_DRAW);
}

void InitTexture(const char* texturePath, unsigned int& textureID, GLenum sourceImageFormat)
{
	//Create and bind Texture
	//unsigned int textureID = 0;
	glGenTextures(1, &textureID);

	//glActiveTexture(GL_TEXTURE0);// activate the texture unit first before binding texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	//Texture Wrapping, s, t (and r if you're using 3D textures) equivalent to x,y,z
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Border color
	/*float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);*/

	//Texture Filtering
	//A common mistake is to set one of the mipmap filtering options as the magnification filter. This doesn't have any effect since mipmaps are primarily used for when textures get downscaled
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Load Texture
	int width = 0;
	int height = 0;
	int nrChannels = 0;
	stbi_set_flip_vertically_on_load(true); //OpenGL expects the 0.0 coordinate on the y-axis to be on the bottom side of the image, but images usually have 0.0 at the top of the y-axis
	unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

	//Generate texture
	if (data != nullptr)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, sourceImageFormat, GL_UNSIGNED_BYTE, data); //Target for generating texture, mipmap level, format to store, width, height, legacy, source image's format, source image's data type
		glGenerateMipmap(GL_TEXTURE_2D); //generate mipmap
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data); //After we're done generating the texture and its corresponding mipmaps, it is good practice to free the image memory
}


Game game;
Shader shader;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

void GameLoop(GLFWwindow* window);

void SetCallbacks(GLFWwindow* window);
void OnWindowResized(GLFWwindow* window, int width, int height);
void OnMouseMove(GLFWwindow* window, double xpos, double ypos);
void OnMouseScroll(GLFWwindow* window, double xOffset, double yOffset);

int main()
{
	bool result = game.Init();
	if (result == false)
	{
		return -1;
	}

	//call this function on every window resize by registering it
	SetCallbacks(game.GetWindow());

	// 1. bind Vertex Array Object
	InitVertexArrayObject(cubeVAO);

	// 2. copy our vertices array in a buffer for OpenGL to use
	InitVertexBuffer(sizeof(cubeVertices), cubeVertices);

	// 3. copy our index array in a element buffer for OpenGL to use
	//InitElementBufferObject();

	// 4. then set the vertex attributes pointers
	InitVertexAttributes();


	/*InitVertexArrayObject(triangle2VAO);
	InitVertexBuffer(sizeof(triangleVertices2), triangleVertices2);
	InitVertexAttributes();*/

	
	shader.InitShader("Shader/shader.vs", "Shader/shader.fs");
	
	InitTexture("Texture/container.jpg", texture1, GL_RGB);
	InitTexture("Texture/awesomeface.png", texture2, GL_RGBA);


	// 5. now draw the object 
	GameLoop(game.GetWindow());

	Clear();

	game.Clear();

	return 0;
}

void GameLoop(GLFWwindow* window)
{
	shader.Use(); // don't forget to activate/use the shader before setting uniforms!

	//Set sampler's texture unit
	shader.SetInt("texture1", 0);
	shader.SetInt("texture2", 1);


	while (glfwWindowShouldClose(window) == false)
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		game.Update(deltaTime);

		game.Render();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		shader.Use();
		shader.SetMatrix("view", game.GetCamera()->GetViewMatrix());
		shader.SetMatrix("projection", game.GetCamera()->GetProjMatrix());

		glBindVertexArray(cubeVAO);
		for (unsigned int i = 0; i < 10; ++i)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

			shader.SetMatrix("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window); //swap the color buffer (a large 2D buffer that contains color values for each pixel in GLFW's window) that is used to render to during this render iteration and show it as output to the screen.
		glfwPollEvents(); //checks if any events are triggered (like keyboard input or mouse movement events), updates the window state, and calls the corresponding functions (which we can register via callback methods)		
	}
}

void SetCallbacks(GLFWwindow* window)
{
	glfwSetFramebufferSizeCallback(window, OnWindowResized);
	glfwSetCursorPosCallback(window, OnMouseMove);
	glfwSetScrollCallback(window, OnMouseScroll);
}

void OnWindowResized(GLFWwindow* window, int width, int height)
{
	//std::cout << "OnWindowResized" << std::endl;
	game.OnWindowResized(width, height);
}

void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
	//std::cout << "OnMouseMove " << "Xpos : " << xpos << " Ypos : " << ypos << std::endl;
	game.OnMouseMove(xpos, ypos);
}

void OnMouseScroll(GLFWwindow* window, double xOffset, double yOffset)
{
	//std::cout << "OnMouseScroll" << "yOffset : " << yOffset << std::endl;
	game.OnMouseScroll(xOffset, yOffset);
}