#pragma once
struct GLFWwindow;

namespace Gorilla
{
	class Camera;
	class Graphics;
}

namespace Engine
{
	class ETime;
	class Input;
	class FlyingCamera;

	class MonkyEngine
	{
	public:

		MonkyEngine(GLFWwindow* aWindow, Gorilla::Camera* aCamera, Input* anInput, Gorilla::Graphics* aGraphics);

		void Update(const float& aDeltaTime);

		Gorilla::Camera* myCamera;

		void CreateCube();
		void CreateSphere();

		bool ShouldSimulate = false;

	private:

		Input* myInput;
		ETime* myTime;
		FlyingCamera* myFlyingCamera;
		Gorilla::Graphics* myGraphics;

		GLFWwindow* myWindow;
	};
}

