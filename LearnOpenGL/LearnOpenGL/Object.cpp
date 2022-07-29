#include "Object.h"

#include <glad/glad.h>

#include "Resources/ResourceManager.h"
#include "Resources/Texture.h"
#include "Resources/Shader.h"
#include "Resources/ModelData.h"

Object::Object()
{

}

Object::~Object()
{

}

void Object::Init(unsigned int objectID, std::shared_ptr<ModelData> modelData, bool hasColor, bool hasTexture)
{
	this->objectID = objectID;
	this->modelData = modelData;
	
	//stores our vertex attribute configuration and which VBO to use
	//when you have multiple objects you want to draw, you first generate/configure all the VAOs (and thus the required VBO and attribute pointers)
	//and store those for later use. The moment we want to draw one of our objects, we take the corresponding VAO, bind it, then draw the object and unbind the VAO again.
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, this->modelData->VBO); //bind buffer at GL_ARRAY_BUFFER 

	InitVertexAttributes(hasColor, hasTexture);



	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Object::Update()
{
	worldMatrix = glm::mat4(1.0f);
	worldMatrix = glm::translate(worldMatrix, position);
	worldMatrix = glm::rotate(worldMatrix, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Object::Render(std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<Shader> shader)
{
	std::shared_ptr<Texture> texture = resourceManager->GetTexture("Container");
	texture->Use(0);
	texture = resourceManager->GetTexture("Awesomeface");
	texture->Use(1);
	
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

void Object::InitVertexAttributes(bool hasColor, bool hasTexture)
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
	
}

void Object::SetPosition(const glm::vec3& position)
{
	this->position = position;
}

