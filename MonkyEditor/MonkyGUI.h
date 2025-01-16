#pragma once
#include <vector>
#include <glm.hpp>

class VirtualObject;
class ResourceHandler;
struct GLFWwindow;

class ResourceEditor;
class ShaderEditor;

class GizmoTest;

namespace Gorilla
{
	class Camera;

}

namespace Chimp
{
	enum class ECurrentEditor
	{
		EObjectHierarchy,
		EShaderEditor,
		EResourceViewer,
		COUNT
	};

	class ObjectEntry;

	class MonkyGUI
	{
	public:
		MonkyGUI(GLFWwindow* aWindow, ResourceHandler* aResourceHandler);
		~MonkyGUI();
		void Render(std::vector<VirtualObject*> someObjects, Gorilla::Camera* aCamera);

	private:

		void UpdateHierarchy(std::vector<VirtualObject*> someObjects, Gorilla::Camera* aCamera);

		void RepopulateEntries(std::vector<VirtualObject*> someObjects);



		std::vector<ObjectEntry*> myObjectEntries;
		ResourceHandler* myResources;

		ECurrentEditor	myCurrentEditor;

		ResourceEditor* myResourceEditor;
		ShaderEditor* myShaderEditor;

		GLFWwindow* myWindow;

		GizmoTest* myGizmo;
		glm::vec2 m_ViewportBounds[2];
		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
	};
}

