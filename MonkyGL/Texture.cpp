#include "Texture.h"
#include "stb_image.h"
#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>

Texture::Texture(const char* aPath)
{
	int Channels = 0;
	Width = 0;
	Height = 0;

	unsigned char* data = stbi_load(aPath, &Width, &Height, &Channels, 0);

	glGenTextures(1, &TextureObject);
	glBindTexture(GL_TEXTURE, TextureObject);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if(data) 
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
	{
		std::cout << "Could not load texture" << std::endl;
	}
	
	stbi_image_free(data);
}
