#ifndef TEXTURE_H
#define TEXTURE

#include <glad/glad.h>

class Texture
{
public :
	void Init(const char* texturePath, bool flipVertical);
	void Use(unsigned int textureUnit);
	void Clear();

private :
	unsigned int textureID = 0;
};

#endif
