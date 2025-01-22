#include "FlyingCamera.h"
#include "Input.h"
#include "Camera.h"
#include "ETime.h"
#include <iostream>

const float Sensitivity = 0.5f;
const float MoveSpeed = 10.0f;

Engine::FlyingCamera::FlyingCamera(Gorilla::Camera* aCamera, Engine::Input* anInput, Engine::ETime* aTime)
{
	myCamera = aCamera;
	myInput = anInput;
	myTime = aTime;

	lastX = 0;
	lastY = 0;

	pitch = 0;
	yaw = 0;
}

void Engine::FlyingCamera::Update(GLFWwindow* aWindow)
{
	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);

	float xpos, ypos;
	xpos = myInput->GetCursorX();
	ypos = myInput->GetCursorY();

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	if (myInput->IsMouseDown(1))
	{
		glfwSetInputMode(aWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else 
	{
		glfwSetInputMode(aWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		return;
	}

	if (myInput->IsKeyDown(GLFW_KEY_W)) velocity.z = 1;
	if (myInput->IsKeyDown(GLFW_KEY_A)) velocity.x = -1;
	if (myInput->IsKeyDown(GLFW_KEY_S)) velocity.z = -1;
	if (myInput->IsKeyDown(GLFW_KEY_D)) velocity.x = 1;

	if (myInput->IsKeyDown(GLFW_KEY_SPACE)) velocity.y = 1;
	if (myInput->IsKeyDown(GLFW_KEY_LEFT_SHIFT)) velocity.y = -1;

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

	std::cout << "-- " << lastX << " " << lastY << " --" << std::endl;
}

void Engine::FlyingCamera::SetCamState(GLFWwindow* aWindow)
{
	Editing = !Editing;

	if (Editing)
	{
		
	}
	else
	{
		
	}
}
