#pragma once
#include <vector>
#include <string>

struct GLFWwindow;
class VirtualObject;

class Mesh;
class Texture;
class Shader;

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

	void CreateVirtualObject(Mesh* aMesh, Texture* aTexture, Shader* aShader);
	void CreateDefaultCube();
	void CreateDefaultSphere();

	std::vector<VirtualObject*> GetObjects();


}


