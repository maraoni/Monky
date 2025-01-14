

#define IMGUI_DEFINE_MATH_OPERATORS
//#include "imgui.h"
//#include "imgui_internal.h"
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
#include "Camera.h"
#include "GizmoTest.h"


#define itoc(a) ((char*)(intptr_t)(a))




Chimp::MonkyGUI::MonkyGUI(GLFWwindow* aWindow, ResourceHandler* aResourceHandler)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigWindowsResizeFromEdges = true;
	io.ConfigWindowsMoveFromTitleBarOnly = true;
	//io.ConfigFlags |= ImGuiConfigFlags_IsTouchScreen | ImGuiConfigFlags_ViewportsEnable |
	//	ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsLight();
	ImGui_ImplGlfw_InitForOpenGL(aWindow, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	myResources = aResourceHandler;
	myWindow = aWindow;
	myCurrentEditor = ECurrentEditor::EObjectHierarchy;

	myShaderEditor = new ShaderEditor();
	myResourceEditor = new ResourceEditor(myResources);

	myGizmo = new GizmoTest();
}



Chimp::MonkyGUI::~MonkyGUI()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

int selectedItem = -1;
bool alwaysTrue = true;

void Chimp::MonkyGUI::Render(std::vector<VirtualObject*> someObjects, Gorilla::Camera* aCamera)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();


	myGizmo->Update(someObjects, aCamera);

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



	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Chimp::MonkyGUI::UpdateHierarchy(std::vector<VirtualObject*> someObjects, Gorilla::Camera* aCamera)
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("ObjectCreation"))
		{
			//if (ImGui::MenuItem("Create Sphere", "")) { Gorilla::CreateDefaultSphere(); }
			//if (ImGui::MenuItem("Create Cube", ""))	{ Gorilla::CreateDefaultCube(); }
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
	myObjectEntries.clear();

	for (size_t i = 0; i < someObjects.size(); i++)
	{
		ObjectEntry* e = new ObjectEntry(someObjects[i]);

		myObjectEntries.push_back(e);
	}
}

void TransformStart(float* cameraView, float* cameraProjection, float* matrix)
{
	//static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
	//static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
	//static bool boundSizing = false;
	//static bool boundSizingSnap = false;

	//if (ImGui::IsKeyPressed(ImGuiKey_T))
	//	mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	//if (ImGui::IsKeyPressed(ImGuiKey_E))
	//	mCurrentGizmoOperation = ImGuizmo::ROTATE;
	//if (ImGui::IsKeyPressed(ImGuiKey_R)) // r Key
	//	mCurrentGizmoOperation = ImGuizmo::SCALE;
	//if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
	//	mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	//ImGui::SameLine();
	//if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
	//	mCurrentGizmoOperation = ImGuizmo::ROTATE;
	//ImGui::SameLine();
	//if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
	//	mCurrentGizmoOperation = ImGuizmo::SCALE;
	//float matrixTranslation[3], matrixRotation[3], matrixScale[3];
	//ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);
	//ImGui::InputFloat3("Tr", matrixTranslation);
	//ImGui::InputFloat3("Rt", matrixRotation);
	//ImGui::InputFloat3("Sc", matrixScale);
	//ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

	//if (mCurrentGizmoOperation != ImGuizmo::SCALE)
	//{
	//	if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
	//		mCurrentGizmoMode = ImGuizmo::LOCAL;
	//	ImGui::SameLine();
	//	if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
	//		mCurrentGizmoMode = ImGuizmo::WORLD;
	//}

	//if (ImGui::IsKeyPressed(ImGuiKey_S))
	//	useSnap = !useSnap;
	//ImGui::Checkbox(" ", &useSnap);
	//ImGui::SameLine();
	//switch (mCurrentGizmoOperation)
	//{
	//case ImGuizmo::TRANSLATE:
	//	ImGui::InputFloat3("Snap", &snap[0]);
	//	break;
	//case ImGuizmo::ROTATE:
	//	ImGui::InputFloat("Angle Snap", &snap[0]);
	//	break;
	//case ImGuizmo::SCALE:
	//	ImGui::InputFloat("Scale Snap", &snap[0]);
	//	break;
	//}

	//ImGuiIO& io = ImGui::GetIO();
	//float viewManipulateRight = io.DisplaySize.x;
	//float viewManipulateTop = 0;
	//static ImGuiWindowFlags gizmoWindowFlags = 0;
	//ImGui::SetNextWindowSize(ImVec2(800, 400), ImGuiCond_Appearing);
	//ImGui::SetNextWindowPos(ImVec2(400, 20), ImGuiCond_Appearing);
	//ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(0.35f, 0.3f, 0.3f));
	//if (useWindow)
	//{
	//	ImGui::Begin("Gizmo", 0, gizmoWindowFlags);
	//	ImGuizmo::SetDrawlist();
	//}
	//float windowWidth = (float)ImGui::GetWindowWidth();
	//float windowHeight = (float)ImGui::GetWindowHeight();

	//if (!useWindow)
	//{
	//	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	//}
	//else
	//{
	//	ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
	//}
	//viewManipulateRight = ImGui::GetWindowPos().x + windowWidth;
	//viewManipulateTop = ImGui::GetWindowPos().y;
	////gizmoWindowFlags = ImGui::IsWindowHovered() && ImGui::IsMouseHoveringRect(window->InnerRect.Min, window->InnerRect.Max) ? ImGuiWindowFlags_NoMove : 0;
	////gizmoWindowFlags = ImGui::IsWindowHovered() && ImGui::IsMouseHoveringRect(myWindow->, window->InnerRect.Max) ? ImGuiWindowFlags_NoMove : 0;

	////ImGuizmo::DrawGrid(cameraView, cameraProjection, identityMatrix, 100.f);
	////ImGuizmo::DrawCubes(cameraView, cameraProjection, &objectMatrix[0][0], gizmoCount);

	//ImGuizmo::ViewManipulate(cameraView, 15, ImVec2(viewManipulateRight - 128, viewManipulateTop), ImVec2(128, 128), 0x10101010);
}

void TransformEnd()
{
	//if (useWindow)
	//{
	//	ImGui::End();
	//}
	//ImGui::PopStyleColor(1);
}
