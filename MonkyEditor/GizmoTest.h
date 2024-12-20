#pragma once
#include <vector>

namespace Gorilla
{
	class Camera;
}
class VirtualObject;

class GizmoTest
{
private:
	float snap[3] = { 1.f, 1.f, 1.f };
	bool useSnap = false;
public:
	GizmoTest();
	void Update(std::vector<VirtualObject*> someObjects, Gorilla::Camera* aCamera);
	bool useWindow = false;


	void EditTransform(float* cameraView, float* cameraProjection, float* matrix);
	void TransformStart(float* cameraView, float* cameraProjection, float* matrix);
	void TransformEnd();
};

