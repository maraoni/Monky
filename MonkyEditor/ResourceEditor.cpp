#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <string>

#include "ResourceEditor.h"
#include <ResourceHandler.h>

#include "Shader.h"

char vPath[256];
char fPath[256];
char ResourceName[256];

ResourceEditor::ResourceEditor(ResourceHandler* aHandler)
{
	myResources = aHandler;
	myCurrentResource = EResource::COUNT;
}

ResourceEditor::~ResourceEditor()
{

}

void ResourceEditor::Update()
{
	if (ImGui::BeginMenu("Create a resource"))
	{
		if (ImGui::MenuItem("Create Texture", "")) { myCurrentResource = EResource::Texture; }
		if (ImGui::MenuItem("Create Shader", "")) { myCurrentResource = EResource::Shader; }
		if (ImGui::MenuItem("Object Mesh", "")) { myCurrentResource = EResource::Mesh; }
		ImGui::EndMenu();
	}

	switch (myCurrentResource)
	{
	case EResource::Texture:

		if (ImGui::Button("Create Texture"))
		{

		}

		break;

	case EResource::Shader:

		ImGui::InputText("Name", ResourceName, IM_ARRAYSIZE(ResourceName));
		ImGui::InputText("vertex path", vPath, IM_ARRAYSIZE(vPath));
		ImGui::InputText("fragment path", fPath, IM_ARRAYSIZE(fPath));

		if (ImGui::Button("Create Shader"))
		{
			std::string FinalVPath = "../Assets/Shaders/";
			std::string FinalFPath = "../Assets/Shaders/";

			std::string FinalName = ResourceName;

			FinalVPath += vPath;
			FinalFPath += fPath;

			myResources->CreateShader(FinalVPath.c_str(), FinalFPath.c_str(), FinalName.c_str());

			memset(vPath, '\0', sizeof(vPath));
			memset(fPath, '\0', sizeof(fPath));
			memset(ResourceName, '\0', sizeof(ResourceName));

			int kek = 0;
		}

		break;

	case EResource::Mesh:

		if (ImGui::Button("Create Mesh"))
		{

		}

		break;

	default:
		break;
	}


	ImGui::BeginChild("Scrolling");

	std::vector<std::string> allResources = myResources->GetAllResources();

	for (int n = 0; n < allResources.size(); n++)
	{
		ImGui::Text(allResources[n].c_str(), n);
	}

	ImGui::EndChild();
}


