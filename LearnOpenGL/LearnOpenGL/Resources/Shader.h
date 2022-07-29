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
#include <unordered_map>

class Shader
{
public :
	void Init(const char* vertexPath, const char* fragmentPath);
	void Use(); // use/activate the shader
	void UnUse();
	void Clear();

	// utility uniform functions
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetVec3(const std::string& name, const glm::vec3& value) const;
	void SetMatrix(const std::string& name, const glm::mat4& value) const;

private :
	unsigned int shaderProgramID = 0; //the program ID

	void ReadShaderFromFile(const char* shaderFilepath, std::string& shaderCode);
	int CreateAndCompileShader(const char* shaderSource, GLenum shaderType);
	void CheckShaderCompile(unsigned int shaderID);
	void CreateAndLinkShaderProgram(unsigned int vertexShaderID, unsigned int fragmentShaderID);
	void CheckShaderProgramLink();
	void DeleteShader(unsigned int vertexShaderID, unsigned int fragmentShaderID);
};
#endif