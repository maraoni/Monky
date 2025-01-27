#include "MonkyGraphics.h"
#include "VirtualObject.h"
#include <iostream>
#include <glad/glad.h>
#include <glfw3.h>
#include <direct.h>
#include "Shader.h"
#include <Windows.h>
#include "Triangle.h"
#include "Cube.h"
#include <glm.hpp>
#include <fwd.hpp>
#include <gtc/matrix_transform.hpp>
#include "Camera.h"
#include "stb_image.h"
#include "Texture.h"
#include "Square.h"

#include <fstream>
#include <sstream>

Gorilla::Graphics::Graphics()
{

}

Gorilla::GorillaInitializeData Gorilla::Graphics::Initialize(int aWidth, int aHeight)
{
	GorillaInitializeData someData;
	someData.aCamera = NULL;
	someData.aWindow = NULL;

	if (!glfwInit())
	{
		std::cout << "Failed to initialize glfw" << std::endl;
		return someData;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(aWidth, aHeight, "Monky", NULL, NULL);
	glfwMakeContextCurrent(window);


	myWidth = aWidth;
	myHeight = aHeight;

	if (window == NULL)
	{
		std::cout << "Failed to initialize window" << std::endl;
		return someData;
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return someData;
	}

	myConcreteTexture = new Texture("../Assets/Images/Grass.png", true);
	myTexture = new Texture("../Assets/Images/Default.png", false);
	myShader = new Shader("../Assets/Shaders/VertexShader.glsl", "../Assets/Shaders/FragmentShader.glsl");
	myBillboard = new Shader("../Assets/Shaders/VertexBillboard.glsl", "../Assets/Shaders/FragmentShader.glsl");

	MonkeyMesh = LoadObjMesh("../Assets/Models/Monkey.obj");
	mySphere = LoadObjMesh("../Assets/Models/Sphere.obj");
	myPlane = LoadObjMesh("../Assets/Models/Plane.obj");

	myCube = LoadObjMesh("../Assets/Models/Cube.obj");
	mySquare = new Square();

	Camera* camera = new Camera(aWidth, aHeight);

	someData.aCamera = camera;
	someData.aWindow = window;

	glEnable(GL_DEPTH_TEST);
	glfwSwapInterval(1);

	return someData;
}

void Gorilla::Graphics::BeginRender(Camera* aCamera)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < myObjects.size(); i++)
	{
		myObjects[i]->Draw(aCamera);
	}

	float time = glfwGetTime();
	myBillboard->SetFloat(time, "time");

	aCamera->CameraUpdate();
}

void Gorilla::Graphics::End()
{
	glfwSwapBuffers(window);
	Input(window);
	glfwPollEvents();


	currentTime = glfwGetTime();
	DeltaTime = currentTime - lastTime;
	lastTime = currentTime;
}


bool Gorilla::Graphics::ShouldClose()
{
	if (glfwWindowShouldClose(window))
	{
		glfwTerminate();

		//delete window;
		delete myShader;
		delete myBillboard;
		delete mySquare;
		delete myTriangle;
		delete myCube;
		delete myTexture;
		delete myConcreteTexture;
		delete MonkeyMesh;
		delete mySphere;
		delete myPlane;

		delete myShader;

		return true;
	}
	return false;
}

void Gorilla::Graphics::Input(GLFWwindow* aWindow)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}


VirtualObject* Gorilla::Graphics::CreateVirtualObject(Mesh* aMesh, Texture* aTexture, Shader* aShader)
{
	VirtualObject* newObject = new VirtualObject(aMesh, aTexture, aShader);
	myObjects.push_back(newObject);
	return newObject;
}

void Gorilla::Graphics::CreateDefaultCube()
{
	VirtualObject* newObject = new VirtualObject(myCube, myTexture, myShader);
	myObjects.push_back(newObject);
}

VirtualObject* Gorilla::Graphics::CreateDefaultCubeReturn()
{
	VirtualObject* newObject = new VirtualObject(myCube, myTexture, myShader);
	myObjects.push_back(newObject);
	return newObject;
}

void Gorilla::Graphics::CreateDefaultSphere()
{
	VirtualObject* newObject = new VirtualObject(mySphere, myTexture, myShader);
	myObjects.push_back(newObject);
}

VirtualObject* Gorilla::Graphics::CreateDefaultSphereReturn()
{
	VirtualObject* newObject = new VirtualObject(mySphere, myTexture, myShader);
	myObjects.push_back(newObject);
	return newObject;
}

std::vector<VirtualObject*> Gorilla::Graphics::GetObjects()
{
	return myObjects;
}
