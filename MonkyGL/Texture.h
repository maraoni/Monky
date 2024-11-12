#pragma once
class Texture
{
public:
	Texture(const char* aPath, const bool& shouldAlpha);

	int Width;
	int Height;

	unsigned int TextureObject;
};

