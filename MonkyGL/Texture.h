#pragma once
class Texture
{
public:
	Texture(const char* aPath);

	int Width;
	int Height;

	unsigned int TextureObject;
};

