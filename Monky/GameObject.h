#pragma once

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

private:
	Banana::Collider* myCollider; // Because we lack a ECS
	VirtualObject* myVirtualObject;
};

