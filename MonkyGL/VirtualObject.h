#pragma once
#include <glm.hpp>

class Mesh;
class Texture;
class Shader;

namespace Gorilla 
{
	class Camera;
}

class VirtualObject
{
public:

	VirtualObject(Mesh* aMesh, Texture* aTexture, Shader* aShader);

	void SetMesh(Mesh& aMesh);
	void SetTexture(Texture& aTexture);
	void SetShader(Shader& aShader);

	void Draw(Gorilla::Camera* aCamera);

	glm::vec3 Position;
	glm::vec3 Scale;
	glm::vec3 Rotation;

	Shader* GetShader();

private:

	Mesh* myMesh;
	Texture* myTexture;
	Shader* myShader;
};

