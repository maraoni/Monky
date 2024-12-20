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

	VirtualObject(std::shared_ptr < Mesh> aMesh, std::shared_ptr < Texture> aTexture, std::shared_ptr < Shader> aShader);
	~VirtualObject();

	void SetMesh(Mesh& aMesh);
	void SetTexture(Texture& aTexture);
	void SetShader(Shader& aShader);

	void Draw(Gorilla::Camera* aCamera);

	glm::vec3 Position;
	glm::vec3 Scale;
	glm::vec3 Rotation;

	Shader* GetShader();

private:

	std::shared_ptr<Mesh>		myMesh;
	std::shared_ptr<Texture>	myTexture;
	std::shared_ptr<Shader>		myShader;
};

