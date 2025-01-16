

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
#include "ImGuizmo.h"



#define itoc(a) ((char*)(intptr_t)(a))



Chimp::MonkyGUI::MonkyGUI(GLFWwindow* aWindow, ResourceHandler* aResourceHandler)
{
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
	ImGuizmo::BeginFrame();

	static bool dockspaceOpen = true;
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
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

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
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

	ImGui::Begin("Viewport");
	auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
	auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
	auto viewportOffset = ImGui::GetWindowPos();
	m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
	m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

	m_ViewportFocused = ImGui::IsWindowFocused();
	m_ViewportHovered = ImGui::IsWindowHovered();

	//Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportHovered);

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

	//uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
	//ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

	//if (ImGui::BeginDragDropTarget())
	//{
	//	if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
	//	{
	//		const wchar_t* path = (const wchar_t*)payload->Data;
	//		//OpenScene(path);
	//	}
	//	ImGui::EndDragDropTarget();
	//}

	// Gizmos

	ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);
	myGizmo->Update(someObjects, aCamera);

	ImGui::End();

	//Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
	//if (selectedEntity && m_GizmoType != -1)
	//{
	//	ImGuizmo::SetOrthographic(false);
	//	ImGuizmo::SetDrawlist();



	//	// Camera

	//	// Runtime camera from entity
	//	// auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
	//	// const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
	//	// const glm::mat4& cameraProjection = camera.GetProjection();
	//	// glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

	//	// Editor camera
	//	const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
	//	glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

	//	// Entity transform
	//	auto& tc = selectedEntity.GetComponent<TransformComponent>();
	//	glm::mat4 transform = tc.GetTransform();

	//	// Snapping
	//	bool snap = Input::IsKeyPressed(Key::LeftControl);
	//	float snapValue = 0.5f; // Snap to 0.5m for translation/scale
	//	// Snap to 45 degrees for rotation
	//	if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
	//		snapValue = 45.0f;

	//	float snapValues[3] = { snapValue, snapValue, snapValue };

	//	ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
	//		(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
	//		nullptr, snap ? snapValues : nullptr);

	//	if (ImGuizmo::IsUsing())
	//	{
	//		glm::vec3 translation, rotation, scale;
	//		Math::DecomposeTransform(transform, translation, rotation, scale);

	//		glm::vec3 deltaRotation = rotation - tc.Rotation;
	//		tc.Translation = translation;
	//		tc.Rotation += deltaRotation;
	//		tc.Scale = scale;
	//	}
	//}






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
