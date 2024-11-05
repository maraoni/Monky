#pragma once

class GLFWwindow;

namespace Gorilla
{
	class Camera;

	struct GorillaInitializeData
	{
		Camera* aCamera;
		GLFWwindow* aWindow;
	};

	GorillaInitializeData Initialize(int aWidth, int aHeight);
	void Render(Camera* aCamera);
	bool ShouldClose();

	void Input(GLFWwindow* aWindow);


}


