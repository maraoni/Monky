#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "MonkyGUI.h"
#include <glm.hpp>
#include "VirtualObject.h"
#include <string>
#include "ObjectEntry.h"

#include "ShaderEditor.h"
#include "ResourceEditor.h"
#include "MonkyGraphics.h"

#define itoc(a) ((char*)(intptr_t)(a))

Chimp::MonkyGUI::MonkyGUI(GLFWwindow* aWindow, ResourceHandler* aResourceHandler)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsLight();
	ImGui_ImplGlfw_InitForOpenGL(aWindow, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	myResources = aResourceHandler;

	myCurrentEditor = ECurrentEditor::EObjectHierarchy;

	myShaderEditor = new ShaderEditor();
	myResourceEditor = new ResourceEditor(myResources);
}



Chimp::MonkyGUI::~MonkyGUI()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

int selectedItem = -1;
bool alwaysTrue = true;

void Chimp::MonkyGUI::Render(std::vector<VirtualObject*> someObjects)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();


	ImGui::Begin("Monky", &alwaysTrue, ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Shader Editor", "Ctrl+O"))		{ myCurrentEditor = ECurrentEditor::EShaderEditor;		}
			if (ImGui::MenuItem("Resource Viewer", "Ctrl+S"))	{ myCurrentEditor = ECurrentEditor::EResourceViewer;	}
			if (ImGui::MenuItem("Object Hierarchy", "Ctrl+W"))	{ myCurrentEditor = ECurrentEditor::EObjectHierarchy;	}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	switch (myCurrentEditor)
	{
	case Chimp::ECurrentEditor::EObjectHierarchy:

		UpdateHierarchy(someObjects);

		break;

	case Chimp::ECurrentEditor::EShaderEditor:

		myShaderEditor->Update();

		break;

	case Chimp::ECurrentEditor::EResourceViewer:

		myResourceEditor->Update();

		break;

	case Chimp::ECurrentEditor::COUNT:
		break;

	default:
		break;
	}

	

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Chimp::MonkyGUI::UpdateHierarchy(std::vector<VirtualObject*> someObjects)
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("ObjectCreation"))
		{
			if (ImGui::MenuItem("Create Sphere", "Ctrl+O")) { Gorilla::CreateDefaultCube(); }
			if (ImGui::MenuItem("Create Cube", "Ctrl+S"))	{ Gorilla::CreateDefaultSphere(); }
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	

	if (someObjects.size() != myObjectEntries.size())
	{
		RepopulateEntries(someObjects);
	}

	ImGui::BeginChild("Scrolling");
	for (size_t i = 0; i < myObjectEntries.size(); i++)
	{
		std::string number = std::to_string(i);
		std::string Title = "Object " + number;

		if (ImGui::Button(Title.c_str()))
		{
			myObjectEntries[i]->Opened = !myObjectEntries[i]->Opened;
			selectedItem = i;
		}

		if (!myObjectEntries[i]->Opened)
		{
			continue;
		}

		if (selectedItem == i)
		{
			myObjectEntries[i]->Update();
		}
		else
		{
			myObjectEntries[i]->Opened = false;
		}
	}
	ImGui::EndChild();
}

void Chimp::MonkyGUI::RepopulateEntries(std::vector<VirtualObject*> someObjects)
{
	for (size_t i = 0; i < myObjectEntries.size(); i++)
	{
		delete myObjectEntries[i];
	}

	myObjectEntries.clear();

	for (size_t i = 0; i < someObjects.size(); i++)
	{
		ObjectEntry* e = new ObjectEntry(someObjects[i]);

		myObjectEntries.push_back(e);
	}
}
