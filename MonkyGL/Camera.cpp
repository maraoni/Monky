#include "Camera.h"
#include "MonkyGraphics.h"
#include <iostream>
#include <gtc/matrix_transform.hpp>
#include <iostream>

glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

Gorilla::Camera::Camera()
{
	myDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	myPosition = glm::vec3(0.0f, 0.0f, 3.0f);
	myUp = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Gorilla::Camera::CameraUpdate()
{
	myRight = glm::normalize(glm::cross(myDirection, WorldUp));
	myUp = glm::cross(myRight, myDirection);
	myView = glm::lookAt(myPosition, myPosition + myDirection, myUp);
}

void Gorilla::Camera::SetPosition(const glm::vec3& aPosition)
{

}

void Gorilla::Camera::SetRotation(const glm::vec3& aRotation)
{

}

void Gorilla::Camera::Move(const glm::vec3& aMove)
{
	myPosition += myDirection * aMove.z;
	myPosition += myRight * aMove.x;
	myPosition += WorldUp * aMove.y;
}

void Gorilla::Camera::SetDirection(const glm::vec3& aDirection)
{
	myDirection = glm::normalize(aDirection);
}
