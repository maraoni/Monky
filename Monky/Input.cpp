#include "Input.h"
#include <glfw3.h>
#include <unordered_map>

std::unordered_map<int, bool> myKeyStates;
float lastX, lastY;

void Engine::Input::KeyCallBack(GLFWwindow* aWindow, int aKey, int aScanCode, int anAction, int aMod)
{
	if (anAction == GLFW_PRESS)
	{
		myKeyStates[aKey] = true;
	}
	else if (anAction == GLFW_RELEASE)
	{
		myKeyStates[aKey] = false;
	}
}

void Engine::Input::MouseCallBack(GLFWwindow* aWindow, double xPos, double yPos)
{
	lastX = xPos;
	lastY = yPos;
}

float Engine::Input::GetCursorX()
{
	return lastX;
}

float Engine::Input::GetCursorY()
{
	return lastY;
}

bool Engine::Input::IsKeyPressed(const int& aKey)
{
	if (myKeyStates[aKey])
	{
		myKeyStates[aKey] = false;
		return true;
	}
	return false;
}

bool Engine::Input::IsKeyDown(const int& aKey)
{
	return glfwGetKey(myWindow, aKey) == GLFW_PRESS;
}

bool Engine::Input::IsMouseDown(const int& aKey)
{
	if (aKey == 0)
	{
		return glfwGetMouseButton(myWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
	}
	else if (aKey == 1)
	{
		return glfwGetMouseButton(myWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
	}
	else
	{
		return false;
	}
}

Engine::Input::Input(GLFWwindow* aWindow)
{
	myWindow = aWindow;

	GLFWvidmode return_struct;

	int width, height = 0;

	glfwGetWindowSize(aWindow, &width, &height);

	lastX = width / 2;
	lastY = height / 2;

	glfwSetKeyCallback(aWindow, KeyCallBack);
	glfwSetCursorPosCallback(aWindow, MouseCallBack);
}
