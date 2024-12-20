#pragma once
#include <vector>

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
	};
}

