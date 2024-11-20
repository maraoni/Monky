#pragma once

class ResourceHandler;

enum class EResource
{
	Texture,
	Shader,
	Mesh,
	COUNT
};


class ResourceEditor
{
public:

	ResourceEditor(ResourceHandler* aHandler);
	~ResourceEditor();

	void Update();

private:

	ResourceHandler* myResources;
	EResource myCurrentResource;
};


