#pragma once
#include <vector>
#include <string>

struct GLFWwindow;
class VirtualObject;

class Mesh;
class Texture;
class Shader;
class Triangle;
class Cube;
class Square;

namespace Gorilla
{
	class Camera;
	struct GorillaInitializeData
	{
		Camera* aCamera;
		GLFWwindow* aWindow;
	};



	class Graphics 
	{
	public:
		Graphics();

		void CreateDefaultCube();
		VirtualObject* CreateDefaultCubeReturn();
		void CreateDefaultSphere();
		VirtualObject* CreateDefaultSphereReturn();

		GorillaInitializeData Initialize(int aWidth, int aHeight);
		void BeginRender(Camera* aCamera);
		void End();
		bool ShouldClose();
		void Input(GLFWwindow* aWindow);

		VirtualObject* CreateVirtualObject(Mesh* aMesh, Texture* aTexture, Shader* aShader);


		std::vector<VirtualObject*> GetObjects();

		GLFWwindow* window;

		Shader* myShader;
		Shader* myBillboard;

		Square* mySquare;
		Triangle* myTriangle;
		Mesh* myCube;

		Texture* myTexture;
		Texture* myConcreteTexture;
		Mesh* MonkeyMesh;

		Mesh* mySphere;
		Mesh* myPlane;

		float myWidth;
		float myHeight;

		float lastTime;
		float currentTime;
		float DeltaTime;

		std::vector<VirtualObject*> myObjects;
		VirtualObject* myBillboardObject = nullptr;
	};
	class Camera;
}


