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

GLFWwindow* window;
Shader* myShader;
Triangle* myTriangle;
Cube* myCube;

float myWidth;
float myHeight;

float lastTime;
float currentTime;
float DeltaTime;


std::vector<glm::vec3> myCubePositions;

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

	HMODULE module = GetModuleHandleW(NULL);
	WCHAR path[MAX_PATH];
	GetModuleFileNameW(module, path, MAX_PATH);

	myShader = new Shader("../Shaders/VertexShader.glsl", "../Shaders/FragmentShader.glsl");
	myTriangle = new Triangle();
	myCube = new Cube();

	Camera* camera = new Camera();

	someData.aCamera = camera;
	someData.aWindow = window;

	glEnable(GL_DEPTH_TEST);

	for (size_t x = 0; x < 10; x++)
	{
		for (size_t y = 0; y < 10; y++)
		{
			myCubePositions.push_back(glm::vec3(x * 2.0f, 0.0f, y * 2.0f));
		}
	}
	//myCubePositions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));

	return someData;
}

void Gorilla::Render(Camera* aCamera)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//myTriangle->Draw(myShader);

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), myWidth / myHeight, 0.1f, 100.0f);

	for (glm::vec3 v : myCubePositions)
	{
		//v.y = glm::cos(((float)glfwGetTime() * 5.5f + (v.x + v.z) * 0.05f)) * 3.0f;
		glm::mat4 trans = glm::mat4(1.0f);

		//trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		//trans = glm::scale(trans, glm::vec3(0.5, glm::cos((float)glfwGetTime()), 0.5));
		trans = glm::translate(trans, v);

		myShader->SetMatrix4(trans, "transform");
		myShader->SetMatrix4(aCamera->myView, "view");
		myShader->SetMatrix4(projection, "projection");

		myCube->Draw(myShader);
	}

	glfwSwapBuffers(window);
	Input(window);
	glfwPollEvents();

	currentTime = glfwGetTime();
	DeltaTime = currentTime - lastTime;
	lastTime = currentTime;

	aCamera->CameraUpdate();
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




