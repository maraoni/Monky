#pragma once

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

		void Update();

	private:

		Engine::Input* myInput;
		Engine::ETime* myTime;
		Gorilla::Camera* myCamera;

		float yaw, pitch;

		double lastX, lastY;
	};
}

