#include "GizmoTest.h"

#include "MonkeyMath.h"
#include <gtc/type_ptr.hpp>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"
#include "ImGuizmo.h"


#include "Camera.h"
#include "VirtualObject.h"
#include <Input.h>


static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);

int lastUsing = 0;


GizmoTest::GizmoTest(Engine::Input* someInput)
{
	input = someInput;
	useSnap = false;
	useWindow = true;
}



void GizmoTest::Update(std::vector<VirtualObject*> someObjects, Gorilla::Camera* aCamera)
{
	ImVec2 window_pos = ImGui::GetWindowPos();
	ImVec2 window_size = ImGui::GetWindowSize();
	ImVec2 window_center = ImVec2(window_pos.x + window_size.x * 0.5f, window_pos.y + window_size.y * 0.5f);

	if (input->IsKeyPressed(GLFW_KEY_R)) gizmoType = ImGuizmo::OPERATION::SCALE;
	else if (input->IsKeyPressed(GLFW_KEY_W)) gizmoType = ImGuizmo::OPERATION::TRANSLATE;
	else if (input->IsKeyPressed(GLFW_KEY_E)) gizmoType = ImGuizmo::OPERATION::ROTATE;

	int kek = 0;

	if (gizmoType != -1)
	{
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();

		float windowWidth = (float)ImGui::GetWindowWidth();
		float windowHeight = (float)ImGui::GetWindowHeight();

		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

		const glm::mat4& cameraView = aCamera->myView;
		glm::mat4 projection = aCamera->myProjection;

		glm::mat4 trans = someObjects[0]->GetTrans();

		ImGuizmo::MODE aMode = ImGuizmo::LOCAL;

		/*if (gizmoType == ImGuizmo::OPERATION::ROTATE)
		{
			aMode = ImGuizmo::WORLD;
		}*/
		ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(projection), (ImGuizmo::OPERATION)gizmoType, aMode, glm::value_ptr(trans));


		if (ImGuizmo::IsUsing)
		{
			glm::vec3 translation, scale, rotation;

			MonkeyMath::DecomposeTransform(trans, translation, rotation, scale);

			glm::vec3 deltaRotation = rotation - someObjects[0]->Rotation;

			int kek = 0;
			ImGuizmo::OPERATION myOperation = (ImGuizmo::OPERATION)gizmoType;

			switch (myOperation)
			{
			case ImGuizmo::OPERATION::ROTATE:
				someObjects[0]->Rotation += deltaRotation;
				break;
			case ImGuizmo::OPERATION::SCALE:
				someObjects[0]->Scale = scale;
				break;
			case ImGuizmo::OPERATION::TRANSLATE:
				someObjects[0]->Position = translation;
				break;
			default:
				break;
			}
			//someObjects[0]->SetTransform(trans);

		}
	}
}

void GizmoTest::TransformStart(float* cameraView, float* cameraProjection, float* matrix)
{
	static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
	static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
	static bool boundSizing = false;
	static bool boundSizingSnap = false;

	if (ImGui::IsKeyPressed(ImGuiKey_T))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	if (ImGui::IsKeyPressed(ImGuiKey_E))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	if (ImGui::IsKeyPressed(ImGuiKey_R)) // r Key
		mCurrentGizmoOperation = ImGuizmo::SCALE;
	if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
		mCurrentGizmoOperation = ImGuizmo::SCALE;
	float matrixTranslation[3], matrixRotation[3], matrixScale[3];
	ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);
	ImGui::InputFloat3("Tr", matrixTranslation);
	ImGui::InputFloat3("Rt", matrixRotation);
	ImGui::InputFloat3("Sc", matrixScale);
	ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

	if (mCurrentGizmoOperation != ImGuizmo::SCALE)
	{
		if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
			mCurrentGizmoMode = ImGuizmo::LOCAL;
		ImGui::SameLine();
		if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
			mCurrentGizmoMode = ImGuizmo::WORLD;
	}

	if (ImGui::IsKeyPressed(ImGuiKey_S))
		useSnap = !useSnap;
	ImGui::Checkbox(" ", &useSnap);
	ImGui::SameLine();
	switch (mCurrentGizmoOperation)
	{
	case ImGuizmo::TRANSLATE:
		ImGui::InputFloat3("Snap", &snap[0]);
		break;
	case ImGuizmo::ROTATE:
		ImGui::InputFloat("Angle Snap", &snap[0]);
		break;
	case ImGuizmo::SCALE:
		ImGui::InputFloat("Scale Snap", &snap[0]);
		break;
	}

	ImGuiIO& io = ImGui::GetIO();
	float viewManipulateRight = io.DisplaySize.x;
	float viewManipulateTop = 0;
	static ImGuiWindowFlags gizmoWindowFlags = 0;
	ImGui::SetNextWindowSize(ImVec2(800, 400), ImGuiCond_Appearing);
	ImGui::SetNextWindowPos(ImVec2(400, 20), ImGuiCond_Appearing);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(0.35f, 0.3f, 0.3f));
	if (useWindow)
	{
		ImGui::Begin("Gizmo", 0, gizmoWindowFlags);
		ImGuizmo::SetDrawlist();
	}
	float windowWidth = (float)ImGui::GetWindowWidth();
	float windowHeight = (float)ImGui::GetWindowHeight();

	if (!useWindow)
	{
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	}
	else
	{
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
	}
	viewManipulateRight = ImGui::GetWindowPos().x + windowWidth;
	viewManipulateTop = ImGui::GetWindowPos().y;
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	gizmoWindowFlags = ImGui::IsWindowHovered() && ImGui::IsMouseHoveringRect(window->InnerRect.Min, window->InnerRect.Max) ? ImGuiWindowFlags_NoMove : 0;

	//ImGuizmo::DrawGrid(cameraView, cameraProjection, identityMatrix, 100.f);
	//ImGuizmo::DrawCubes(cameraView, cameraProjection, &objectMatrix[0][0], gizmoCount);
	//ImGuizmo::ViewManipulate(cameraView, camDistance, ImVec2(viewManipulateRight - 128, viewManipulateTop), ImVec2(128, 128), 0x10101010);
}

void GizmoTest::TransformEnd()
{
	if (useWindow)
	{
		ImGui::End();
	}
	ImGui::PopStyleColor(1);
}

void GizmoTest::EditTransform(float* cameraView, float* cameraProjection, float* matrix)
{
	ImGuiIO& io = ImGui::GetIO();
	float windowWidth = (float)ImGui::GetWindowWidth();
	float windowHeight = (float)ImGui::GetWindowHeight();
	if (!useWindow)
	{
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	}
	else
	{
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
	}
	ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, useSnap ? &snap[0] : NULL);
}
