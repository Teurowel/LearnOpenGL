#include "Texture.h"

#include "stb_image.h"
#include <iostream>

void Texture::Init(const char* texturePath, bool flipVertical)
{
	//Load Texture
	int width = 0;
	int height = 0;
	int nrChannels = 0;
	stbi_set_flip_vertically_on_load(flipVertical); //OpenGL expects the 0.0 coordinate on the y-axis to be on the bottom side of the image, but images usually have 0.0 at the top of the y-axis
	unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
	//Generate texture
	if (data != nullptr)
	{
		GLenum sourceImageFormat;
		if(nrChannels == 1)
		{
			sourceImageFormat = GL_RED;
		}
		else if(nrChannels == 3)
		{
			sourceImageFormat = GL_RGB;
		}
		else if(nrChannels == 4)
		{
			sourceImageFormat = GL_RGBA;
		}
		
		//Create and bind Texture
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		//Give texture data to GPU
		glTexImage2D(GL_TEXTURE_2D, 0, sourceImageFormat, width, height, 0, sourceImageFormat, GL_UNSIGNED_BYTE, data); //Target for generating texture, mipmap level, format to store, width, height, legacy, source image's format, source image's data type
		glGenerateMipmap(GL_TEXTURE_2D); //generate mipmap

		// set the texture wrapping/filtering options (on the currently bound texture object)
		//Texture Wrapping, s, t (and r if you're using 3D textures) equivalent to x,y,z
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//Border color
		/*float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);*/

		//Texture Filtering
		//A common mistake is to set one of the mipmap filtering options as the magnification filter. This doesn't have any effect since mipmaps are primarily used for when textures get downscaled
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		stbi_image_free(data); //After we're done generating the texture and its corresponding mipmaps, it is good practice to free the image memory
	}
	else
	{
		std::cout << "Failed to load texture " << texturePath << std::endl;
		stbi_image_free(data); //After we're done generating the texture and its corresponding mipmaps, it is good practice to free the image memory
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Use(unsigned int textureUnit)
{
	switch (textureUnit)
	{
	case 0 :
		glActiveTexture(GL_TEXTURE0);
		break;

	case 1:
		glActiveTexture(GL_TEXTURE1);
		break;
	}
	
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::Clear()
{
	glDeleteTextures(1, &textureID);
}
