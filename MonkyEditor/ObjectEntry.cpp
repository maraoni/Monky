#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ObjectEntry.h"
#include "VirtualObject.h"
#include <iostream>
#include <gtc/constants.hpp>

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

void Chimp::ObjectEntry::Update()
{
	if(myObject == nullptr) 
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


	ImGui::InputFloat3("Position ", *pos);
	ImGui::SliderFloat3("Rotation ", *rot, -glm::pi<float>(), glm::pi<float>());
}

