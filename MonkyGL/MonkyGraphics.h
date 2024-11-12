#pragma once
#include <vector>
#include <string>

struct GLFWwindow;
class VirtualObject;

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

	std::string LoadObjRaw(const char* aPath);



	void Input(GLFWwindow* aWindow);

	std::vector<VirtualObject*> GetObjects();


}


