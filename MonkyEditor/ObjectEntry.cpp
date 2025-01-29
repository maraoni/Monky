#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ObjectEntry.h"
#include "VirtualObject.h"
#include <iostream>
#include <gtc/constants.hpp>
#include "Shader.h"
#include <stdio.h>
#include <string.h>
#include "GameObject.h"
#include <DirectXCollision.h>

Chimp::ObjectEntry::ObjectEntry(GameObject* anObject) : myObject(anObject)
{
	Opened = false;
}

Chimp::ObjectEntry::~ObjectEntry()
{
	myObject = nullptr;
}


void Chimp::ObjectEntry::Update()
{
	if (&myObject == nullptr)
	{
		std::cout << "[CHIMP] Object entry has null object" << std::endl;
		return;
	}

	float* pos[] =
	{
		&myObject->GetVirtual()->Position.x,
		&myObject->GetVirtual()->Position.y,
		&myObject->GetVirtual()->Position.z,
	};

	float* rot[] =
	{
		&myObject->GetVirtual()->Rotation.x,
		&myObject->GetVirtual()->Rotation.y,
		&myObject->GetVirtual()->Rotation.z,
	};


	float* scale[] =
	{
		&myObject->GetVirtual()->Scale.x,
		&myObject->GetVirtual()->Scale.y,
		&myObject->GetVirtual()->Scale.z,
	};

	ImGui::InputFloat3("Position ", *pos);
	ImGui::InputFloat3("Scale ", *scale);
	ImGui::InputFloat3("Rotation ", *rot);

	Banana::Collider* c = myObject->GetCollider();
	if (c)
	{
		ColliderData data = myObject->GetData();;

		if (myObject->IsSphereCollider())
		{

			ImGui::LabelText("Sphere Collider", "");

			if (ImGui::Button("Remove collider"))
			{
				myObject->RemoveCollider();
			}

			ImGui::InputFloat("Radius", &data.Radius);
			ImGui::Checkbox("Gravity", &data.HasGravity);

			myObject->SetData(data);

		}
		else if (myObject->IsBoxCollider())
		{

			ImGui::LabelText("Box Collider", "");

			if (ImGui::Button("Remove collider"))
			{
				myObject->RemoveCollider();
			}

			ImGui::LabelText("Extents", "");

			ImGui::InputFloat("x ", &data.Extents.x);
			ImGui::InputFloat("y ", &data.Extents.y);
			ImGui::InputFloat("z ", &data.Extents.z);

			ImGui::Checkbox("Gravity", &data.HasGravity);

			myObject->SetData(data);

		}
	}
	else
	{
		if (ImGui::BeginMenu("Add collider"))
		{
			if (ImGui::MenuItem("Sphere collider", ""))
			{
				myObject->CreateSphereCollider(1.0f);
			}
			if (ImGui::MenuItem("Cube collider", ""))
			{
				myObject->CreateBoxCollider(glm::vec3(1.0f, 1.0f, 1.0f));
			}
			ImGui::EndMenu();
		}
	}

}

