#ifndef OBJECT_H
#define OBJECT_H

class Object
{
public :
	Object();
	~Object();

	void Init(float* vertices);
	void Update();
	void Render();
	void Clear();

	void InitVertexAttributes(bool hasColor, bool hasTexture);

private :
	float* vertices = nullptr;

	unsigned int VAO = 0;
	unsigned int VBO = 0;
	unsigned int EBO = 0;


};

#endif

