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
	Gorilla::Graphics* graphics = new Gorilla::Graphics();

	Gorilla::GorillaInitializeData RenderData = graphics->Initialize(1280, 720);
	Engine::MonkyEngine* engine = new Engine::MonkyEngine(RenderData.aWindow, RenderData.aCamera);
	Chimp::MonkyGUI* Gui = new Chimp::MonkyGUI(RenderData.aWindow, resources);



	glfwSetInputMode(RenderData.aWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	float lastTime = 0;
	float currentTime = 0;
	float delta = 0;



	while (!graphics->ShouldClose())
	{
		std::vector<VirtualObject*> objects = graphics->GetObjects();
		currentTime = glfwGetTime();
		delta = currentTime - lastTime;
		lastTime = currentTime;

		try
		{
			graphics->BeginRender(engine->myCamera);
			Gui->Render(objects, engine->myCamera);



			graphics->End();
		}
		catch (int anError)
		{
			std::cout << anError << std::endl;
		}

		engine->Update(delta);
	}

	return 0;
}





