#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Source.h"

GLFWwindow* window = nullptr;
unsigned int vertexShaderID = 0;
unsigned int vertexShaderID2 = 0;

unsigned int fragmentShaderID = 0;
unsigned int fragmentShaderID2 = 0;

unsigned int triangleVAO = 0;
unsigned int triangle2VAO = 0;

unsigned int shaderProgramID = 0;
unsigned int shaderProgramID2 = 0;

bool isWireFrameMode = false;

float triangleVertices[] = {
	 -1.0f, -0.5f, 0.0f,
	 0.0f, -0.5f, 0.0f,
	 -0.5f,  0.5f, 0.0f
};

float triangleVertices2[] = {
	 0.0f, -0.5f, 0.0f,
	 1.0f, -0.5f, 0.0f,
	 0.5f,  0.5f, 0.0f
};

float rectangleVertices[] = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
};
unsigned int rectangleIndices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};

void InitGLFW()
{
	glfwInit(); //initialize GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //what option we want to configure, https://www.glfw.org/docs/latest/window.html#window_hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

bool CreateGLFWWindow(GLFWwindow** window)
{
	*window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (*window == NULL)
	{
		std::cout << "Failed to create GLFW widnow" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(*window);
	return true;
}

bool InitGLAD()
{
	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == false)
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	return true;
}

void InitViewport(int x, int y, int width, int height)
{
	//tell OpenGL the size of the rendering window
	//processed coordinates in OpenGL are between -1 and 1 so we effectively map from the range (-1 to 1) to (0, 800) and (0, 600).
	//(-0.5,0.5) would be mapped to (200,450)
	glViewport(x, y, width, height); //location of the lower left corner, width and height of the rendering window in pixels
}

void OnWindowResized(GLFWwindow* window, int width, int height)
{
	std::cout << "OnWindowResized" << std::endl;
	InitViewport(0, 0, width, height);
}

void EnableWireFrameMode(bool enable)
{
	if (enable == true)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void ProcessInput(GLFWwindow* window)
{
	//if it's not pressed, glfwGetKey returns GLFW_RELEASE
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	else if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
	{
		isWireFrameMode = !isWireFrameMode;
		EnableWireFrameMode(isWireFrameMode);
	}
}

void RenderLoop(GLFWwindow* window)
{
	while (glfwWindowShouldClose(window) == false)
	{
		ProcessInput(window);

		//rendering commnads here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //set clear color
		glClear(GL_COLOR_BUFFER_BIT); //clear Color_Buffer using clear color

		glUseProgram(shaderProgramID);
		glBindVertexArray(triangleVAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(shaderProgramID2);
		glBindVertexArray(triangle2VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwPollEvents(); //checks if any events are triggered (like keyboard input or mouse movement events), updates the window state, and calls the corresponding functions (which we can register via callback methods)
		glfwSwapBuffers(window); //swap the color buffer (a large 2D buffer that contains color values for each pixel in GLFW's window) that is used to render to during this render iteration and show it as output to the screen.
		
	}
}

void Clear()
{
	glfwTerminate();
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
	unsigned int VBO = 0;
	glGenBuffers(1, &VBO); //Generate Buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //bind buffer at GL_ARRAY_BUFFER 
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW); //set data to GL_ARRAY_BUFFER, stored data in memory on graphics card
}

void InitVertexAttributes()
{
	//Each vertex attribute takes its data from memory managed by a VBO and which VBO it takes its data from (you can have multiple VBOs) is determined by the VBO currently bound to GL_ARRAY_BUFFER when calling glVertexAttribPointer
	//we specified how OpenGL should interpret the vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //location in Vertexshader, size of vertex attribute(vec3), normalize data, stride(space between data), offset where position data begin
	glEnableVertexAttribArray(0);
}

void InitElementBufferObject()
{
	//An EBO is a buffer, just like a vertex buffer object, that stores indices that OpenGL uses to decide what vertices to draw. This so called indexed drawing
	unsigned int EBO = 0;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleIndices), rectangleIndices, GL_STATIC_DRAW);
}

void CheckShaderCompile(unsigned int shaderID)
{
	int  success;
	char infoLog[512];
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

void CheckShaderProgramLink(unsigned int shaderProgramID)
{
	int  success;
	char infoLog[512];
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADERPROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}
}

void CreateAndCompileShader(unsigned int& shaderID, const char* shaderSource, GLenum shaderType)
{
	shaderID = glCreateShader(shaderType); //Create vertex shader
	glShaderSource(shaderID, 1, &shaderSource, nullptr); //attach shader source 
	glCompileShader(shaderID); //compile vertex shader
}

void InitVertexShader(unsigned int& vertexShaderID)
{
	const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

	//In order for OpenGL to use the shader it has to dynamically compile it at run-time from its source code
	CreateAndCompileShader(vertexShaderID, vertexShaderSource, GL_VERTEX_SHADER);
	CheckShaderCompile(vertexShaderID);
}

void InitFragmentShader(unsigned int& fragmentShaderID, const char* fragmentShaderSource)
{
	CreateAndCompileShader(fragmentShaderID, fragmentShaderSource, GL_FRAGMENT_SHADER);
	CheckShaderCompile(fragmentShaderID);
}

void InitShaderProgram(unsigned int& shaderProgramID, unsigned int vertexShaderID, unsigned int fragmentShaderID)
{
	//A shader program object is the final linked version of multiple shaders combined. 
	//To use the recently compiled shaders we have to link them to a shader program object and then activate this shader program when rendering objects. 
	//The activated shader program's shaders will be used when we issue render calls.
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShaderID);
	glAttachShader(shaderProgramID, fragmentShaderID);
	glLinkProgram(shaderProgramID);
	glUseProgram(shaderProgramID); //Activate shader program, Every shader and rendering call after glUseProgram will now use this program object (and thus the shaders).

	CheckShaderProgramLink(shaderProgramID);

	glDeleteShader(vertexShaderID); //once we've linked them into the program object; we no longer need them anymore:
	glDeleteShader(fragmentShaderID);
}




int main()
{
	InitGLFW();

	bool result = CreateGLFWWindow(&window);
	if (result == false)
	{
		return -1;
	}

	result = InitGLAD();
	if (result == false)
	{
		return -1;
	}

	InitViewport(0, 0, 800, 600);

	//call this function on every window resize by registering it
	glfwSetFramebufferSizeCallback(window, OnWindowResized);




	// 1. bind Vertex Array Object
	InitVertexArrayObject(triangleVAO);

	// 2. copy our vertices array in a buffer for OpenGL to use
	InitVertexBuffer(sizeof(triangleVertices), triangleVertices);

	// 3. copy our index array in a element buffer for OpenGL to use
	//InitElementBufferObject();

	// 4. then set the vertex attributes pointers
	InitVertexAttributes();


	InitVertexArrayObject(triangle2VAO);
	InitVertexBuffer(sizeof(triangleVertices2), triangleVertices2);
	InitVertexAttributes();




	// 4. use our shader program when we want to render an object
	InitVertexShader(vertexShaderID);
	InitFragmentShader(fragmentShaderID, "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0 "
	);
	InitShaderProgram(shaderProgramID, vertexShaderID, fragmentShaderID);
	

	InitVertexShader(vertexShaderID2);
	InitFragmentShader(fragmentShaderID2, "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"FragColor = vec4(0.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0 "
	);
	InitShaderProgram(shaderProgramID2, vertexShaderID2, fragmentShaderID2);

	

	// 5. now draw the object 
	RenderLoop(window);

	Clear();
	return 0;
}

