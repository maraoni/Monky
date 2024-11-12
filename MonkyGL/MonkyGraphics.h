#pragma once
struct GLFWwindow;

namespace Gorilla
{
	class Camera;

	struct GorillaInitializeData
	{
		Camera* aCamera;
		GLFWwindow* aWindow;
	};

	GorillaInitializeData Initialize(int aWidth, int aHeight);
	void BeginRender(Camera* aCamera);
	void End();
	bool ShouldClose();

	void Input(GLFWwindow* aWindow);


}


