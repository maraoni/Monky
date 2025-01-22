#include "VirtualObject.h"
#include <glad/glad.h>
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "MonkeyMath.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/quaternion.hpp>

VirtualObject::VirtualObject(Mesh* aMesh, Texture* aTexture, Shader* aShader)
{
	myMesh = aMesh;
	myTexture = aTexture;
	myShader = aShader;

	if (!aMesh || !aTexture || !aShader)
	{
		int kek = 0;
	}


	Position = glm::vec3(0, 0, 0);
	Rotation = glm::vec3(0, 0, 0);
	Scale = glm::vec3(1, 1, 1);


}

VirtualObject::~VirtualObject()
{
	myMesh = nullptr;
	myTexture = nullptr;
	myShader = nullptr;
}

void VirtualObject::SetMesh(Mesh& aMesh)
{
	//myMesh = &aMesh;
}

void VirtualObject::SetTexture(Texture& aTexture)
{
	//myTexture = &aTexture;
}

void VirtualObject::SetShader(Shader& aShader)
{
	//myShader = &aShader;
}

void VirtualObject::SetRotation(const glm::vec3& aRotation)
{
	Rotation = aRotation;
}

void VirtualObject::SetTransform(const glm::mat4& aTransform)
{
	glm::vec3 translation, rotation, scale;
	MonkeyMath::DecomposeTransform(aTransform, translation, rotation, scale);

	Position = translation;
	Rotation = rotation;
	Scale = scale;
}

void VirtualObject::Draw(Gorilla::Camera* aCamera)
{
	if (!myMesh || !myTexture || !myShader)
	{
		int kek = 0;
	}

	glm::mat4 trans = glm::mat4(1.0f);

	trans = glm::translate(trans, Position);

	trans = glm::rotate(trans, Rotation.x, glm::vec3(1, 0, 0));
	trans = glm::rotate(trans, Rotation.y, glm::vec3(0, 1, 0));
	trans = glm::rotate(trans, Rotation.z, glm::vec3(0, 0, 1));

	trans = glm::scale(trans, Scale);

	if (myTexture)
	{
		glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
		glBindTexture(GL_TEXTURE_2D, myTexture->TextureObject);
	}


	myShader->SetMatrix4(trans, "transform");
	myShader->SetMatrix4(aCamera->myView, "view");
	myShader->SetMatrix4(aCamera->myProjection, "projection");
	myMesh->Draw(myShader);

	glBindTexture(GL_TEXTURE_2D, 0);
}

glm::mat4 VirtualObject::GetTrans()
{
	glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

	return glm::translate(glm::mat4(1.0f), Position)
		* rotation
		* glm::scale(glm::mat4(1.0f), Scale);
}

Shader* VirtualObject::GetShader()
{
	return nullptr;
	//return myShader;
}

