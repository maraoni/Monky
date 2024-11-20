#include "ResourceHandler.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "ObjLoader.h"
#include "ResourceHandler.h"

ResourceHandler::ResourceHandler()
{

}

ResourceHandler::~ResourceHandler()
{

}

void ResourceHandler::CreateShader(const char* aVertexPath, const char* aFragmentPath, std::string aName)
{
	Shader* newShader = new Shader(aVertexPath, aFragmentPath);

	if (newShader->myShaderProgram != 0)
	{
		myShaders.emplace(aName, newShader);
	}
}

void ResourceHandler::CreateTexture(const char* aTexturePath, std::string aName)
{
	Texture* newTexture = new Texture(aTexturePath, false);

	myTextures.emplace(aName, newTexture);
}

void ResourceHandler::CreateMesh(const char* aModelPath, std::string aName)
{
	Mesh* newMesh = Gorilla::LoadObjMesh(aModelPath);

	myMeshes.emplace(aName, newMesh);
}

Shader* ResourceHandler::GetShader(std::string aName)
{
	return myShaders[aName];
}

Texture* ResourceHandler::GetTexture(std::string aName)
{
	return myTextures[aName];
}

Mesh* ResourceHandler::GetMesh(std::string aName)
{
	return myMeshes[aName];
}

std::vector<std::string> ResourceHandler::GetAllResources()
{
	std::vector<std::string> ReturnVector;

	for (auto& m : myMeshes)
	{
		ReturnVector.push_back(m.first);
	}

	for (auto& s : myShaders)
	{
		ReturnVector.push_back(s.first);
	}

	for (auto& t : myTextures)
	{
		ReturnVector.push_back(t.first);
	}

	return ReturnVector;
}
