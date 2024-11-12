
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>

#include <iostream>
#include <glfw3.h>
#include "MonkyEngine.h"

#include "MonkyGraphics.h"



int main()
{
	Gorilla::GorillaInitializeData RenderData = Gorilla::Initialize(1280, 720);

	Engine::MonkyEngine* engine = new Engine::MonkyEngine(RenderData.aWindow, RenderData.aCamera);
	glfwSetInputMode(RenderData.aWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	float lastTime = 0;
	float currentTime = 0;
	float delta = 0;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(RenderData.aWindow, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	while(!Gorilla::ShouldClose()) 
	{
		currentTime = glfwGetTime();
		delta = currentTime - lastTime;
		lastTime = currentTime;

		Gorilla::BeginRender(engine->myCamera); // GL

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		ImGui::Begin("Hello");
		ImGui::Button("Cool button");
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		Gorilla::End(); // GL

		engine->Update(delta);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	return 0;
}





