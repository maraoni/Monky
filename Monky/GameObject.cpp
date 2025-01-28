#include "GameObject.h"
#include "Collisions.h"
#include "VirtualObject.h"

GameObject::GameObject(VirtualObject* anObject, Banana::Collider* aCollider)
{
	myVirtualObject = anObject;
	myCollider = aCollider;
}

GameObject::~GameObject()
{
	myCollider = nullptr;
}

void GameObject::Update()
{
	if(myCollider != nullptr) 
	{
		myCollider->position = myVirtualObject->Position;
	}
}

VirtualObject* GameObject::GetVirtual()
{
	return myVirtualObject;
}

Banana::Collider* GameObject::GetCollider()
{
	return myCollider;
}

void GameObject::CreateSphereCollider(const float& aRadius)
{
	myCollider = new Banana::SphereCollider(myVirtualObject->Position, aRadius);
}

void GameObject::CreateBoxCollider(const glm::vec3& someExtents)
{
	myCollider = new Banana::BoxCollider(myVirtualObject->Position, someExtents);
}


