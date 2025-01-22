#include "MonkyEngine.h"

#include "Camera.h"
#include <glfw3.h>
#include "Input.h"
#include "ETime.h"
#include <glm.hpp>
#include "FlyingCamera.h"



Engine::MonkyEngine::MonkyEngine(GLFWwindow* aWindow, Gorilla::Camera* aCamera, Input* anInput)
{
	myInput = anInput;
	myTime = new ETime();
	myWindow = aWindow;

	myFlyingCamera = new FlyingCamera(aCamera, myInput, myTime);
	myCamera = aCamera;
}

void Engine::MonkyEngine::Update(const float& aDeltaTime)
{
	myFlyingCamera->Update(myWindow);
	myTime->UpdateDeltaTime(aDeltaTime);
}


