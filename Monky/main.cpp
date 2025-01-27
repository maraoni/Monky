#include "MonkyGUI.h"
#include "MonkyEngine.h"
#include "MonkyGraphics.h"
#include "MonkyPhysics.h"

#include "ResourceHandler.h"
#include "OpenGLFrameBuffer.h"
#include "Input.h"

#include <glfw3.h>
#include <iostream>
#include <stdio.h>
#include "VirtualObject.h"
#include <vector>

int main()
{
	ResourceHandler* resources = new ResourceHandler();
	Gorilla::Graphics* graphics = new Gorilla::Graphics();

	int Width, Height;
	Width = 1280;
	Height = 720;

	Gorilla::GorillaInitializeData RenderData = graphics->Initialize(Width, Height);
	Gorilla::FrameBufferSpecification FrameBufferSpec;

	FrameBufferSpec.width = Width;
	FrameBufferSpec.height = Height;

	Engine::Input* input = new Engine::Input(RenderData.aWindow);

	Gorilla::OpenGLFrameBuffer* buffer = new Gorilla::OpenGLFrameBuffer(FrameBufferSpec);
	Engine::MonkyEngine* engine = new Engine::MonkyEngine(RenderData.aWindow, RenderData.aCamera, input, graphics);
	Chimp::MonkyGUI* Gui = new Chimp::MonkyGUI(RenderData.aWindow, resources, input, engine);
	Banana::MonkyPhysics* Physics = new Banana::MonkyPhysics(engine);



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
			buffer->Bind();
			graphics->BeginRender(engine->myCamera);
			buffer->Unbind();
			Gui->Render(engine->GetGameObjects(), engine->myCamera, buffer);

			if(engine->IsSimulating()) 
			{
				Physics->Simulate(delta);
			}

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





