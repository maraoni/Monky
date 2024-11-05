#include <iostream>
#include "MonkyEngine.h"
#include "MonkyGraphics.h"
#include <glfw3.h>

int main()
{

	Gorilla::GorillaInitializeData RenderData = Gorilla::Initialize(840, 600);

	Engine::MonkyEngine* engine = new Engine::MonkyEngine(RenderData.aWindow, RenderData.aCamera);

	float lastTime = 0;
	float currentTime = 0;
	float delta = 0;

	while(!Gorilla::ShouldClose()) 
	{
		currentTime = glfwGetTime();
		delta = currentTime - lastTime;
		lastTime = currentTime;
		Gorilla::Render(engine->myCamera);
		engine->Update(delta);
	}

	return 0;
}





