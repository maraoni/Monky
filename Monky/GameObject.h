#pragma once
#include <glm.hpp>

struct ColliderData
{
	float Mass;
	float Radius;
	glm::vec3 Extents;
	glm::vec3 Center;
	bool HasGravity;
	bool IsKinematic;
};

namespace Banana
{
	class Collider;
	class SphereCollider;
	class BoxCollider;
}

class VirtualObject;

class GameObject
{
public:
	GameObject(VirtualObject* anObject, Banana::Collider* aCollider);
	~GameObject();

	void Update();
	VirtualObject* GetVirtual();



	// collision properties -- because we lack ECS
	Banana::Collider* GetCollider();

	void				CreateSphereCollider(const float& aRadius);
	void				CreateBoxCollider(const glm::vec3& someExtents);

	void				SetData(const ColliderData& someData);
	const				ColliderData& GetData();
	bool				IsSphereCollider();
	bool				IsBoxCollider();

	void				RemoveCollider();

private:
	Banana::Collider* myCollider;
	VirtualObject* myVirtualObject;
};



