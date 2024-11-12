#include "MonkyGraphics.h"

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
#include <vector>
#include "stb_image.h"
#include "Texture.h"
#include "Square.h"
#include "VirtualObject.h"

GLFWwindow* window;

Shader* myShader;
Shader* myBillboard;

Square* mySquare;
Triangle* myTriangle;
Cube* myCube;

Texture* myTexture;
Texture* myConcreteTexture;

float myWidth;
float myHeight;

float lastTime;
float currentTime;
float DeltaTime;

std::vector<VirtualObject*> myObjects;

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

	myTexture = new Texture("../Assets/Images/Default.png");
	myConcreteTexture = new Texture("../Assets/Images/Concrete.png");
	myShader = new Shader("../Assets/Shaders/VertexShader.glsl", "../Assets/Shaders/FragmentShader.glsl");
	myBillboard = new Shader("../Assets/Shaders/VertexBillboard.glsl", "../Assets/Shaders/FragmentShader.glsl");
	myCube = new Cube();
	mySquare = new Square();

	Camera* camera = new Camera(aWidth, aHeight);

	someData.aCamera = camera;
	someData.aWindow = window;

	glEnable(GL_DEPTH_TEST);

	for (size_t x = 0; x < 10; x++)
	{
		for (size_t y = 0; y < 10; y++)
		{
			VirtualObject* b = new VirtualObject(*myCube, *myTexture, *myShader);
			myObjects.push_back(b);

			b->Position = glm::vec3(x * 2.0f, 0, y * 2.0f);
		}
	}

	myObjects[67]->SetTexture(*myConcreteTexture);

	VirtualObject* billboard = new VirtualObject(*mySquare, *myConcreteTexture, *myBillboard);
	myObjects.push_back(billboard);

	billboard->Position = glm::vec3(0, 5, 0);

	return someData;
}

void Gorilla::BeginRender(Camera* aCamera)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (VirtualObject* g : myObjects)
	{
		g->Draw(aCamera);
	}

	myObjects[67]->Rotation.y = glm::cos(glfwGetTime()) * 2;

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




