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

float myWidth;
float myHeight;

float lastTime;
float currentTime;
float DeltaTime;

std::vector<VirtualObject*> myObjects;
VirtualObject* myBillboardObject = nullptr;

Mesh* CreateObjMesh(std::string anObj);

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

	Mesh* tree = CreateObjMesh(LoadObjRaw("../Assets/Models/TreeTrunk.obj"));

	myCube = new Cube();
	mySquare = new Square();

	Camera* camera = new Camera(aWidth, aHeight);

	someData.aCamera = camera;
	someData.aWindow = window;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);



	VirtualObject* billboard = new VirtualObject(mySquare, myConcreteTexture, myBillboard);
	myObjects.push_back(billboard);

	billboard->Position = glm::vec3(-2.0f, 0, 0.0f);

	myBillboardObject = billboard;

	for (size_t x = 0; x < 10; x++)
	{
		for (size_t y = 0; y < 10; y++)
		{
			VirtualObject* b = new VirtualObject(myCube, myTexture, myShader);
			myObjects.push_back(b);

			b->Position = glm::vec3(x * 2.0f, 0, y * 2.0f);
		}
	}


	//myObjects[67]->SetTexture(*myConcreteTexture);



	return someData;
}

void Gorilla::BeginRender(Camera* aCamera)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	for (int i = 0; i < myObjects.size(); i++)
	{
		myObjects[i]->Draw(aCamera);
	}
	//myBillboardObject->Position.y = glm::cos(glfwGetTime());

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



std::string Gorilla::LoadObjRaw(const char* aPath)
{

	std::string ObjFile;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		shaderFile.open(aPath);

		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();

		shaderFile.close();
		ObjFile = shaderStream.str();

		return ObjFile;
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "Could not load shader file from path - " << aPath << "\n";
		return "";
	}
}


Mesh* CreateObjMesh(std::string anObj)
{
	std::vector<glm::vec3> verts;

	std::vector<std::string> lines;
	auto ss = std::stringstream{ anObj };

	for (std::string line; std::getline(ss, line, '\n');)
	{
		lines.push_back(line);
	}

	for (size_t i = 0; i < lines.size(); i++)
	{
		std::vector<std::string> words;
		auto ss = std::stringstream{ anObj };
		//lines[]
	}


	return nullptr;
}
