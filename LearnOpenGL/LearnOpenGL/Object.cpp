#include "Object.h"

#include <glad/glad.h>

Object::Object()
{

}

Object::~Object()
{

}

void Object::Init(float* vertices)
{
	this->vertices = vertices;
}

void Object::Update()
{

}

void Object::Render()
{

}

void Object::Clear()
{

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
