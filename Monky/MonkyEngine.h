#pragma once
class GLFWwindow;

namespace Gorilla
{
	class Camera;
}

namespace Engine
{
	class ETime;
	class Input;
	class FlyingCamera;

	class MonkyEngine
	{
	public:

		MonkyEngine(GLFWwindow* aWindow, Gorilla::Camera* aCamera);

		void Update(const float& aDeltaTime);

		Gorilla::Camera* myCamera;
	private:

		Input* myInput;
		ETime* myTime;
		FlyingCamera* myFlyingCamera;

		GLFWwindow* myWindow;
	};
}

