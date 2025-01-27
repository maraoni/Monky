#pragma once
#include <vector>

namespace Gorilla
{
	class Camera;
}

namespace Engine 
{
	class Input;
}

class VirtualObject;
class GameObject;

class GizmoTest
{
private:
	float snap[3] = { 1.f, 1.f, 1.f };
	bool useSnap = false;
	int gizmoType = -1;
	int orientMode = 0;
	Engine::Input* input;

public:
	GizmoTest(Engine::Input* someInput);
	void Update(GameObject* SelectedObject, Gorilla::Camera* aCamera);
	bool useWindow = false;

	void SetMode(const int& aMode);
};

