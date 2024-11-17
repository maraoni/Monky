#pragma once
#include <vector>

class VirtualObject;
struct GLFWwindow;

namespace Chimp
{
	class ObjectEntry;
	class MonkyGUI
	{
	public:
		MonkyGUI(GLFWwindow* aWindow);
		~MonkyGUI();
		void Render(std::vector<VirtualObject*> someObjects);

	private:
		void RepopulateEntries(std::vector<VirtualObject*> someObjects);
		std::vector<ObjectEntry*> myObjectEntries;
	};
}

