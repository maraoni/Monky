#pragma once
#include <glm.hpp>
#include <memory>

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
	~VirtualObject();

	void SetMesh(Mesh& aMesh);
	void SetTexture(Texture& aTexture);
	void SetShader(Shader& aShader);

	void SetRotation(const glm::vec3& aRotation);

	void SetTransform(const glm::mat4& aTransform);

	void Draw(Gorilla::Camera* aCamera);

	glm::vec3 Position;
	glm::vec3 Scale;
	glm::vec3 Rotation;

	glm::mat4 GetTrans();

	Shader* GetShader();

private:

	Mesh* myMesh;
	Texture* myTexture;
	Shader* myShader;
};

