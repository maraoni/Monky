#include "MonkyEngine.h"

#include "MonkyGraphics.h"
#include "Camera.h"
#include <glfw3.h>
#include "Input.h"
#include "ETime.h"
#include <glm.hpp>
#include "FlyingCamera.h"



Engine::MonkyEngine::MonkyEngine(GLFWwindow* aWindow, Gorilla::Camera* aCamera, Input* anInput, Gorilla::Graphics* aGraphics)
{
	myInput = anInput;
	myTime = new ETime();
	myWindow = aWindow;

	myFlyingCamera = new FlyingCamera(aCamera, myInput, myTime);
	myCamera = aCamera;
	myGraphics = aGraphics;
}

void Engine::MonkyEngine::Update(const float& aDeltaTime)
{
	myFlyingCamera->Update(myWindow);
	myTime->UpdateDeltaTime(aDeltaTime);
}

void Engine::MonkyEngine::CreateCube()
{
	GameObject* cube = new GameObject(myGraphics->CreateDefaultCubeReturn(), nullptr);
	myGameObjects.push_back(cube);
}

void Engine::MonkyEngine::CreateSphere()
{
	GameObject* sphere = new GameObject(myGraphics->CreateDefaultSphereReturn(), nullptr);
	myGameObjects.push_back(sphere);
}

void Engine::MonkyEngine::StartSimulation()
{
	ShouldSimulate = true;
}
void Engine::MonkyEngine::StopSimulation()
{
	ShouldSimulate = false;
}

const bool& Engine::MonkyEngine::IsSimulating()
{
	return ShouldSimulate;
}

std::vector<GameObject*> Engine::MonkyEngine::GetGameObjects()
{
	return myGameObjects;
}


