#include "MonkyGUI.h"
#include "MonkyEngine.h"
#include "MonkyGraphics.h"
#include "ResourceHandler.h"

#include <glfw3.h>
#include <iostream>
#include <stdio.h>
#include "VirtualObject.h"
#include <vector>

int main()
{
	ResourceHandler* resources = new ResourceHandler();
	Gorilla::GorillaInitializeData RenderData = Gorilla::Initialize(1280, 720);
	Engine::MonkyEngine* engine = new Engine::MonkyEngine(RenderData.aWindow, RenderData.aCamera);
	Chimp::MonkyGUI* Gui = new Chimp::MonkyGUI(RenderData.aWindow, resources);


	glfwSetInputMode(RenderData.aWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	float lastTime = 0;
	float currentTime = 0;
	float delta = 0;

	std::vector<VirtualObject*> objects;

	while (!Gorilla::ShouldClose())
	{
		objects = Gorilla::GetObjects();
		currentTime = glfwGetTime();
		delta = currentTime - lastTime;
		lastTime = currentTime;

		Gorilla::BeginRender(engine->myCamera);
		Gui->Render(objects);
		Gorilla::End();

		engine->Update(delta);
	}

	return 0;
}





