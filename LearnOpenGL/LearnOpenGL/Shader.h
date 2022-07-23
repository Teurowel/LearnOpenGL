#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public :
	unsigned int shaderProgramID = 0; //the program ID

	Shader();

	void InitShader(const char* vertexPath, const char* fragmentPath);
	void Use(); // use/activate the shader

	// utility uniform functions
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetMatrix(const std::string& name, glm::mat4& value) const;

private :
	void ReadShaderFromFile(const char* shaderFilepath, std::string& shaderCode);
	int CreateAndCompileShader(const char* shaderSource, GLenum shaderType);
	void CheckShaderCompile(unsigned int shaderID);
	void CreateAndLinkShaderProgram(unsigned int vertexShaderID, unsigned int fragmentShaderID);
	void CheckShaderProgramLink();
	void DeleteShader(unsigned int vertexShaderID, unsigned int fragmentShaderID);
};
#endif