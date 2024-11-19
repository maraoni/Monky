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

Chimp::ObjectEntry::ObjectEntry()
{
	myObject = nullptr;
	Opened = false;
}

Chimp::ObjectEntry::ObjectEntry(VirtualObject* anObject)
{
	myObject = anObject;
	Opened = false;
}

Chimp::ObjectEntry::~ObjectEntry()
{

}

char vPath[256];
char fPath[256];

void Chimp::ObjectEntry::Update()
{
	if (myObject == nullptr)
	{
		std::cout << "[CHIMP] Object entry has null object" << std::endl;
		return;
	}

	float* pos[] =
	{
		&myObject->Position.x,
		&myObject->Position.y,
		&myObject->Position.z,
	};

	float* rot[] =
	{
		&myObject->Rotation.x,
		&myObject->Rotation.y,
		&myObject->Rotation.z,
	};


	float* scale[] =
	{
		&myObject->Scale.x,
		&myObject->Scale.y,
		&myObject->Scale.z,
	};

	ImGui::InputFloat3("Position ", *pos);
	ImGui::InputFloat3("Scale ", *scale);
	ImGui::SliderFloat3("Rotation ", *rot, -glm::pi<float>(), glm::pi<float>());

	// -- Move to dedicated shader tool
	ImGui::InputText("vertex path", vPath, IM_ARRAYSIZE(vPath));
	ImGui::InputText("fragment path", fPath, IM_ARRAYSIZE(fPath));

	if (ImGui::Button("Load Shader"))
	{
		std::string FinalVPath = "../Assets/Shaders/";
		std::string FinalFPath = "../Assets/Shaders/";

		FinalVPath += vPath;
		FinalFPath += fPath;

		myObject->GetShader()->RecompileShader(FinalVPath.c_str(), FinalFPath.c_str());

		memset(vPath, '\0', sizeof(vPath));
		memset(fPath, '\0', sizeof(fPath));
	}
	// -- Move to dedicated shader tool

}

