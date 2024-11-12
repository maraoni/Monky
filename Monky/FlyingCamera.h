#pragma once

struct GLFWwindow;

namespace Gorilla
{
	class Camera;
}

namespace Engine
{
	class Input;
	class ETime;

	class FlyingCamera
	{
	public:

		FlyingCamera(Gorilla::Camera* aCamera, Engine::Input* anInput, Engine::ETime* aTime);

		void Update(GLFWwindow* aWindow);


	private:

		void SetCamState(GLFWwindow* aWindow);
		Engine::Input* myInput;
		Engine::ETime* myTime;
		Gorilla::Camera* myCamera;

		float yaw, pitch;

		bool Editing = false;

		double lastX, lastY;
	};
}

