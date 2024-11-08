#include "MonkyEngine.h"
#include "Camera.h"
#include <glfw3.h>
#include "Input.h"
#include "ETime.h"
#include <glm.hpp>
#include "FlyingCamera.h"

Engine::MonkyEngine::MonkyEngine(GLFWwindow* aWindow, Gorilla::Camera* aCamera)
{
	myInput = new Input(aWindow);
	myTime = new ETime();

	myFlyingCamera = new FlyingCamera(aCamera, myInput, myTime);
	myCamera = aCamera;

	glfwSetInputMode(aWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Engine::MonkyEngine::Update(const float& aDeltaTime)
{
	myFlyingCamera->Update();
	myTime->UpdateDeltaTime(aDeltaTime);
}


