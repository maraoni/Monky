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



void GizmoTest::Update(VirtualObject* SelectedObject, Gorilla::Camera* aCamera)
{
	if(SelectedObject == nullptr) 
	{
		return;
	}
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

		glm::mat4 trans = SelectedObject->GetTrans();

		ImGuizmo::MODE aMode = (ImGuizmo::MODE)orientMode;

		ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(projection), (ImGuizmo::OPERATION)gizmoType, aMode, glm::value_ptr(trans));


		if (ImGuizmo::IsUsing)
		{
			glm::vec3 translation, scale, rotation;

			MonkeyMath::DecomposeTransform(trans, translation, rotation, scale);

			glm::vec3 deltaRotation = rotation - SelectedObject->Rotation;

			int kek = 0;
			ImGuizmo::OPERATION myOperation = (ImGuizmo::OPERATION)gizmoType;

			switch (myOperation)
			{
			case ImGuizmo::OPERATION::ROTATE:
				SelectedObject->Rotation += deltaRotation;
				break;
			case ImGuizmo::OPERATION::SCALE:
				SelectedObject->Scale = scale;
				break;
			case ImGuizmo::OPERATION::TRANSLATE:
				SelectedObject->Position = translation;
				break;
			default:
				break;
			}
		}
	}
}

void GizmoTest::SetMode(const int& aMode)
{
	orientMode = aMode;
}

