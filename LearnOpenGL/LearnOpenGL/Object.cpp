#include "Object.h"

#include "Resources/ResourceManager.h"
#include "Resources/Shader/Shader.h"
#include "Resources/Model.h"

Object::Object()
{

}

Object::~Object()
{

}

void Object::Init(unsigned int objectID, std::shared_ptr<Model> model)
{
	this->objectID = objectID;
	this->model = model;
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
	if(shader->GetShaderType() == ResourceManager::UnLit)
	{
		shader->SetVec3("unLitColor", unLitColor);	
	}
	
	
	shader->SetMatrix("model", worldMatrix);

	model->Draw(shader);
}

void Object::Clear()
{	
	
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

