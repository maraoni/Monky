

#define IMGUI_DEFINE_MATH_OPERATORS
//#include "imgui.h"
//#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "OpenGLFrameBuffer.h"

#include "MonkyGUI.h"
#include <glm.hpp>
#include "VirtualObject.h"
#include <string>
#include "ObjectEntry.h"
#include "MonkyEngine.h"

#include "ShaderEditor.h"
#include "ResourceEditor.h"
#include "MonkyGraphics.h"
#include "Camera.h"
#include "GizmoTest.h"
#include "ImGuizmo.h"
#include <Input.h>


#define itoc(a) ((char*)(intptr_t)(a))



Chimp::MonkyGUI::MonkyGUI(GLFWwindow* aWindow, ResourceHandler* aResourceHandler, Engine::Input* someInput, Engine::MonkyEngine* anEngine)
{
	myEngine = anEngine;
	SelectedObject = nullptr;
	myInput = someInput;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigWindowsResizeFromEdges = true;
	io.ConfigWindowsMoveFromTitleBarOnly = true;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable/* |
		ImGuiConfigFlags_NavEnableKeyboard*/;

	ImGui::StyleColorsLight();
	ImGui_ImplGlfw_InitForOpenGL(aWindow, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	myResources = aResourceHandler;
	myWindow = aWindow;
	myCurrentEditor = ECurrentEditor::EObjectHierarchy;

	myShaderEditor = new ShaderEditor();
	myResourceEditor = new ResourceEditor(myResources);

	myGizmo = new GizmoTest(someInput);
}



Chimp::MonkyGUI::~MonkyGUI()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

int selectedItem = -1;
bool alwaysTrue = true;

void Chimp::MonkyGUI::Render(std::vector<VirtualObject*> someObjects, Gorilla::Camera* aCamera, Gorilla::OpenGLFrameBuffer* aFrameBuffer)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	static bool dockspaceOpen = true;
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;


	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	ImGuiIO& io = ImGui::GetIO();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
	ImGui::PopStyleVar();

	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Viewport", &alwaysTrue, ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Space"))
		{
			if (ImGui::MenuItem("World space", "")) { myGizmo->SetMode(ImGuizmo::MODE::WORLD); }
			if (ImGui::MenuItem("Local space", "")) { myGizmo->SetMode(ImGuizmo::MODE::LOCAL); }
			ImGui::EndMenu();
		}

		if (myEngine->IsSimulating())
		{
			if (ImGui::Button("Stop >>"))
			{
				myEngine->StopSimulation();
			}
		}
		else
		{
			if (ImGui::Button("Play ||"))
			{
				myEngine->StartSimulation();
			}
		}

		ImGui::EndMenuBar();
	}

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

	glm::vec2 viewPortSize = { viewportPanelSize.x, viewportPanelSize.y };

	if (m_ViewportSize != viewPortSize)
	{
		m_ViewportSize = viewPortSize;
		aFrameBuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);
		aCamera->Resize(m_ViewportSize.x, m_ViewportSize.y);
	}



	auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
	auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
	auto viewportOffset = ImGui::GetWindowPos();
	m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
	m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

	m_ViewportFocused = ImGui::IsWindowFocused();
	m_ViewportHovered = ImGui::IsWindowHovered();


	uint32_t textureID = aFrameBuffer->GetColorAttatchmentRendererID();
	ImGui::Image(textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2(0, 0), ImVec2(1, -1));

	ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

	if (!myEngine->IsSimulating())
	{
		myGizmo->Update(SelectedObject, aCamera);
	}

	ImGui::PopStyleVar();
	ImGui::End();

	ImGui::Begin("Monky", &alwaysTrue, ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Shader Editor", "Ctrl+O")) { myCurrentEditor = ECurrentEditor::EShaderEditor; }
			if (ImGui::MenuItem("Resource Viewer", "Ctrl+S")) { myCurrentEditor = ECurrentEditor::EResourceViewer; }
			if (ImGui::MenuItem("Object Hierarchy", "Ctrl+W")) { myCurrentEditor = ECurrentEditor::EObjectHierarchy; }
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	switch (myCurrentEditor)
	{
	case Chimp::ECurrentEditor::EObjectHierarchy:

		UpdateHierarchy(someObjects, aCamera);

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

	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void Chimp::MonkyGUI::UpdateHierarchy(std::vector<VirtualObject*> someObjects, Gorilla::Camera* aCamera)
{



	if (ImGui::BeginMenuBar())
	{

		if (ImGui::BeginMenu("ObjectCreation"))
		{
			if (ImGui::MenuItem("Create Cube", ""))
			{
				myEngine->CreateCube();
			}
			if (ImGui::MenuItem("Create Sphere", ""))
			{
				myEngine->CreateSphere();
			}
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

			if (myObjectEntries[i]->Opened == false)
			{
				SelectedObject = nullptr;
			}

			selectedItem = i;
		}

		if (!myObjectEntries[i]->Opened)
		{
			continue;
		}

		if (selectedItem == i)
		{
			SelectedObject = myObjectEntries[i]->myObject;
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
	myObjectEntries.clear();

	for (size_t i = 0; i < someObjects.size(); i++)
	{
		ObjectEntry* e = new ObjectEntry(someObjects[i]);

		myObjectEntries.push_back(e);
	}
}
