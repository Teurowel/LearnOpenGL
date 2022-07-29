#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void Shader::Init(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexCode = "";
	ReadShaderFromFile(vertexPath, vertexCode);

	std::string fragmentCode = "";
	ReadShaderFromFile(fragmentPath, fragmentCode);

	//convert string to c type string
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// 2. compile shaders
	//vertex shader
	unsigned int vertexShaderID = CreateAndCompileShader(vShaderCode, GL_VERTEX_SHADER);
	CheckShaderCompile(vertexShaderID);

	//Fragment Shader
	unsigned int fragmentShaderID = CreateAndCompileShader(fShaderCode, GL_FRAGMENT_SHADER);
	CheckShaderCompile(fragmentShaderID);

	//Shader Program
	CreateAndLinkShaderProgram(vertexShaderID, fragmentShaderID);
	CheckShaderProgramLink();

	//once we've linked them into the program object; we no longer need them anymore:
	DeleteShader(vertexShaderID, fragmentShaderID);	
}

void Shader::Use()
{
	glUseProgram(shaderProgramID);
}

void Shader::UnUse()
{
	glUseProgram(0);
}

void Shader::Clear()
{
	glDeleteProgram(shaderProgramID);
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(shaderProgramID, name.c_str()), value);
}

void Shader::SetMatrix(const std::string& name, glm::mat4& value) const
{
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::ReadShaderFromFile(const char* shaderFilepath, std::string& shaderCode)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::ifstream shaderFile;

	// ensure ifstream objects can throw exceptions:
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		//open files
		shaderFile.open(shaderFilepath);
		std::stringstream shaderStream;

		// read file's buffer contents into streams
		shaderStream << shaderFile.rdbuf();

		//close file handlers
		shaderFile.close();

		//convert stream into string
		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
}

int Shader::CreateAndCompileShader(const char* shaderSource, GLenum shaderType)
{
	unsigned int shaderID = glCreateShader(shaderType); //Create vertex shader
	glShaderSource(shaderID, 1, &shaderSource, nullptr); //attach shader source 
	glCompileShader(shaderID); //compile vertex shader
	return shaderID;
}

void Shader::CheckShaderCompile(unsigned int shaderID)
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

void Shader::CreateAndLinkShaderProgram(unsigned int vertexShaderID, unsigned int fragmentShaderID)
{
	//A shader program object is the final linked version of multiple shaders combined. 
	//To use the recently compiled shaders we have to link them to a shader program object and then activate this shader program when rendering objects. 
	//The activated shader program's shaders will be used when we issue render calls.
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShaderID);
	glAttachShader(shaderProgramID, fragmentShaderID);
	glLinkProgram(shaderProgramID);
}

void Shader::CheckShaderProgramLink()
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

void Shader::DeleteShader(unsigned int vertexShaderID, unsigned int fragmentShaderID)
{
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}
