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


GLFWwindow* window;

Shader* myShader;
Shader* myBillboard;

Square* mySquare;
Triangle* myTriangle;
Cube* myCube;

Texture* myTexture;
Texture* myConcreteTexture;
Mesh* MonkeyMesh;

float myWidth;
float myHeight;

float lastTime;
float currentTime;
float DeltaTime;

std::vector<VirtualObject*> myObjects;
VirtualObject* myBillboardObject = nullptr;


Gorilla::GorillaInitializeData Gorilla::Initialize(int aWidth, int aHeight)
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

	myCube = new Cube();
	mySquare = new Square();

	Camera* camera = new Camera(aWidth, aHeight);

	someData.aCamera = camera;
	someData.aWindow = window;

	glEnable(GL_DEPTH_TEST);
	glfwSwapInterval(1);

	for (size_t i = 0; i < 3; i++)
	{
		VirtualObject* monkey = new VirtualObject(MonkeyMesh, myTexture, myShader);
		myObjects.push_back(monkey);
		monkey->Position = glm::vec3(i * 2.0f, 0.0f, 0);
	}

	return someData;
}

void Gorilla::BeginRender(Camera* aCamera)
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

void Gorilla::End()
{
	glfwSwapBuffers(window);
	Input(window);
	glfwPollEvents();

	currentTime = glfwGetTime();
	DeltaTime = currentTime - lastTime;
	lastTime = currentTime;
}


bool Gorilla::ShouldClose()
{
	if (glfwWindowShouldClose(window))
	{
		glfwTerminate();
		return true;
	}
	return false;
}

void Gorilla::Input(GLFWwindow* aWindow)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

std::vector<VirtualObject*> Gorilla::GetObjects()
{
	return myObjects;
}
