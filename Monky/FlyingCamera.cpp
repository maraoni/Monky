#include "FlyingCamera.h"
#include "Input.h"
#include "Camera.h"
#include <glfw3.h>
#include <glm.hpp>
#include "ETime.h"
#include <iostream>

const float Sensitivity = 0.5f;
const float MoveSpeed = 10.0f;

Engine::FlyingCamera::FlyingCamera(Gorilla::Camera* aCamera, Engine::Input* anInput, Engine::ETime* aTime)
{
	myCamera = aCamera;
	myInput = anInput;
	myTime = aTime;
}

void Engine::FlyingCamera::Update()
{
	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);

	if (myInput->IsKeyDown(GLFW_KEY_W)) velocity.z = 1;
	if (myInput->IsKeyDown(GLFW_KEY_A)) velocity.x = -1;
	if (myInput->IsKeyDown(GLFW_KEY_S)) velocity.z = -1;
	if (myInput->IsKeyDown(GLFW_KEY_D)) velocity.x = 1;

	if (myInput->IsKeyDown(GLFW_KEY_SPACE)) velocity.y = 1;
	if (myInput->IsKeyDown(GLFW_KEY_LEFT_SHIFT)) velocity.y = -1;

	float xpos, ypos;
	xpos = myInput->GetCursorX();
	ypos = myInput->GetCursorY();

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	//*myTime->DeltaTime()
	//	* myTime->DeltaTime()

	xoffset *= Sensitivity;
	yoffset *= Sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	myCamera->Move(velocity * MoveSpeed * myTime->DeltaTime());
	myCamera->SetDirection(direction);
}
