#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <glm/glm.hpp>

class Game;

class Shader
{
public :
	void Init(const Game* game, const std::string& vertexPath, const std::string& fragmentPath);
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
	const Game* game = nullptr;
	
	unsigned int shaderProgramID = 0; //the program ID

	void ReadShaderFromFile(const std::string& shaderFilepath, std::string& shaderCode);
	int CreateAndCompileShader(const char* shaderSource, GLenum shaderType);
	void CheckShaderCompile(unsigned int shaderID);
	void CreateAndLinkShaderProgram(unsigned int vertexShaderID, unsigned int fragmentShaderID);
	void CheckShaderProgramLink();
	void DeleteShader(unsigned int vertexShaderID, unsigned int fragmentShaderID);
};
#endif