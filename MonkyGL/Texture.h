#pragma once
class Texture
{
public:
	Texture(const char* aPath, bool shouldAlpha);

	int Width = 0;
	int Height = 0;

	unsigned int TextureObject = 0;
};

