#include "Object.h"

#include <glad/glad.h>
#include <string>

#include "Resources/ResourceManager.h"
#include "Resources/Texture.h"
#include "Resources/Shader.h"
#include "Resources/ModelData.h"
#include "Camera.h"
#include "Game.h"
#include "Light.h"

Object::Object()
{

}

Object::~Object()
{

}

void Object::Init(unsigned int objectID,
                  std::shared_ptr<ModelData> modelData,
                  std::shared_ptr<Shader> shader,
                  std::shared_ptr<std::list<const char*>> textureKeys,
                  bool hasColor, bool hasTexture, bool hasNormalVector, Game* game)
{
	this->objectID = objectID;
	this->modelData = modelData;
	this->shader = shader;
	this->textureKeys = textureKeys;
	this->game = game;
	
	//stores our vertex attribute configuration and which VBO to use
	//when you have multiple objects you want to draw, you first generate/configure all the VAOs (and thus the required VBO and attribute pointers)
	//and store those for later use. The moment we want to draw one of our objects, we take the corresponding VAO, bind it, then draw the object and unbind the VAO again.
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, this->modelData->VBO); //bind buffer at GL_ARRAY_BUFFER 

	InitVertexAttributes(hasColor, hasTexture, hasNormalVector);



	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Object::Update()
{
	worldMatrix = glm::mat4(1.0f);
	worldMatrix = glm::translate(worldMatrix, position);
	worldMatrix = glm::rotate(worldMatrix, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	worldMatrix = glm::scale(worldMatrix, scale);
}

void Object::Render()
{
	shader->Use();

	const std::shared_ptr<Camera> camera = game->GetCamera(); 
	shader->SetMatrix("view", camera->GetViewMatrix());
	shader->SetMatrix("projection", camera->GetProjMatrix());
	shader->SetVec3("viewPos", camera->GetPosition());

	
	const std::shared_ptr<Light> light = game->GetLight();
	shader->SetVec3("light.position", light->GetLightPos());
	shader->SetVec3("light.ambient", light->GetAmbientColor());
	shader->SetVec3("light.diffuse", light->GetDiffuseColor());
	shader->SetVec3("light.specular", light->GetSpecularColor());
	
	
	shader->SetVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
	shader->SetVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
	shader->SetVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
	shader->SetFloat("material.shininess", 32.0f);

	
	
	if(textureKeys != nullptr)
	{
		std::string shaderTextureName = "texture";
		std::string shaderTextureNum = "0";

		int num = 0;
		for(auto textureKey : (*textureKeys))
		{
			shaderTextureName = "texture";
			shaderTextureNum = std::to_string(num);
			shader->SetInt(shaderTextureName + shaderTextureNum, num);

			game->GetResourceManager()->GetTexture(textureKey)->Use(num);

			++num;
		}	
	}
	
	
	shader->SetMatrix("model", worldMatrix);

	glBindVertexArray(VAO);

	glDrawArrays(GL_TRIANGLES, 0, modelData->vertexCount);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	shader->UnUse();
}

void Object::Clear()
{	
	glDeleteVertexArrays(1, &VAO);
}

void Object::InitVertexAttributes(bool hasColor, bool hasTexture, bool hasNormalVector)
{
	int index = 0;
	int stride = 3;
	int offset = 0;

	if (hasColor)
	{
		stride += 3;
	}
	if (hasTexture)
	{
		stride += 2;
	}
	if(hasNormalVector)
	{
		stride += 3;
	}

	//Position attribute
	glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)offset); //location in Vertexshader, size of vertex attribute(vec3), normalize data, stride(space between data), offset where position data begin
	glEnableVertexAttribArray(index);
	++index;
	offset += 3;

	if (hasColor)
	{
		//Color
		glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
		glEnableVertexAttribArray(index);
		++index;
		offset += 3;
	}
	
	if (hasTexture)
	{
		//Texture
		glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
		glEnableVertexAttribArray(index);
		++index;
		offset += 2;
	}

	if(hasNormalVector)
	{
		//Texture
		glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
		glEnableVertexAttribArray(index);
		++index;
		offset += 3;
	}
	
}

void Object::SetPosition(const glm::vec3& position)
{
	this->position = position;
}

void Object::SetScale(const glm::vec3& scale)
{
	this->scale = scale;
}

std::shared_ptr<Shader> Object::GetShader() const
{
	return shader;
}

