#pragma once
#include <unordered_map>
#include <vector>
#include <string>

class Texture;
class Shader;
class Mesh;

class ResourceHandler
{
public:

	ResourceHandler();
	~ResourceHandler();

	void CreateShader	(const char* aVertexPath,	const char* aFragmentPath, std::string aName);
	void CreateTexture	(const char* aTexturePath,	std::string aName);
	void CreateMesh		(const char* aModelPath,	std::string aName);

	Shader*		GetShader(std::string	aName);
	Texture*	GetTexture(std::string	aName);
	Mesh*		GetMesh(std::string		aName);

	std::vector<std::string> GetAllResources();

private:

	std::unordered_map<std::string, Texture* >	myTextures;
	std::unordered_map<std::string, Shader*	>	myShaders;
	std::unordered_map<std::string, Mesh*	>	myMeshes;
};

