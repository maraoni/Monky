#pragma once


struct GLFWwindow;

namespace Engine
{
	class Input
	{
	public:
		Input(GLFWwindow* aWindow);
		bool IsKeyPressed(const int& aKey);
		bool IsKeyDown(const int& aKey);
		float GetCursorX();
		float GetCursorY();
	private:
		static void KeyCallBack(GLFWwindow* aWindow, int aKey, int aScanCode, int anAction, int aMod);
		static void MouseCallBack(GLFWwindow* aWindow, double xPos, double yPos);

		GLFWwindow* myWindow;

	};
}

