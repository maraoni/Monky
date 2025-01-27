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

