#pragma once
#include <glm.hpp>

namespace Banana 
{
	class Collider;
}

class VirtualObject;

class GameObject
{
public:
	GameObject(VirtualObject* anObject, Banana::Collider* aCollider);
	~GameObject();

	void Update();

	VirtualObject* GetVirtual();
	Banana::Collider* GetCollider();

	void CreateSphereCollider(const float& aRadius);
	void CreateBoxCollider(const glm::vec3& someExtents);

private:
	Banana::Collider* myCollider; // Because we lack a ECS
	VirtualObject* myVirtualObject;
};

