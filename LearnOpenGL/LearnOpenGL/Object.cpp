#include "Object.h"

#include <glad/glad.h>

#include "Resources/ResourceManager.h"
#include "Resources/Shader.h"
#include "Resources/ModelData.h"
#include "Game.h"
#include "Resources/Material.h"

Object::Object()
{

}

Object::~Object()
{

}

void Object::Init(unsigned int objectID,
                  std::shared_ptr<ModelData> modelData,
                  std::shared_ptr<Material> material,
                  bool hasColor, bool hasTexture, bool hasNormalVector, Game* game)
{
	this->objectID = objectID;
	this->modelData = modelData;
	this->material = material;
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

void Object::Render(std::shared_ptr<Shader> shader)
{
	shader->SetVec3("unLitColor", unLitColor);
	
	if(material != nullptr)
	{
		const std::shared_ptr<ResourceManager> resourceManager = game->GetResourceManager(); 
		shader->SetInt("material.diffuse", 0);
		//resourceManager->GetTexture(material->GetDiffuseTextureKey())->Use(0);

		shader->SetInt("material.specular", 1);
		//resourceManager->GetTexture(material->GetSpecularTextureKey())->Use(1);
		
		shader->SetFloat("material.shininess", material->GetShininess());
	}
	
	shader->SetMatrix("model", worldMatrix);

	glBindVertexArray(VAO);

	glDrawArrays(GL_TRIANGLES, 0, modelData->vertexCount);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
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

void Object::SetUnLitColor(const glm::vec3& color)
{
	this->unLitColor = color;
}

