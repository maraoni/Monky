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
	if (myCollider != nullptr)
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

void GameObject::SetData(const ColliderData& someData)
{
	if (myCollider->isOf<Banana::SphereCollider>())
	{
		Banana::SphereCollider* sc = dynamic_cast<Banana::SphereCollider*>(myCollider);
		sc->mass		= someData.Mass;
		sc->center		= someData.Center;
		sc->hasGravity	= someData.HasGravity;
		sc->radius		= someData.Radius;
		sc->isKinematic = someData.IsKinematic;
	}
	else if (myCollider->isOf<Banana::BoxCollider>())
	{
		Banana::BoxCollider* bc = dynamic_cast<Banana::BoxCollider*>(myCollider);
		bc->mass		= someData.Mass;
		bc->center		= someData.Center;
		bc->hasGravity  = someData.HasGravity;
		bc->extents		= someData.Extents;
		bc->isKinematic = someData.IsKinematic;
	}
}

const ColliderData& GameObject::GetData()
{
	ColliderData d;

	if (myCollider->isOf<Banana::SphereCollider>())
	{
		Banana::SphereCollider* sc = dynamic_cast<Banana::SphereCollider*>(myCollider);
		d.Mass			= sc->mass;
		d.Center		= sc->center;
		d.HasGravity	= sc->hasGravity;
		d.Radius		= sc->radius;
		d.IsKinematic	= sc->isKinematic;
	}
	else if (myCollider->isOf<Banana::BoxCollider>())
	{
		Banana::BoxCollider* bc = dynamic_cast<Banana::BoxCollider*>(myCollider);
		d.Mass			= bc->mass;
		d.Center		= bc->center;
		d.HasGravity	= bc->hasGravity;
		d.Extents		= bc->extents;
		d.IsKinematic	= bc->isKinematic;
	}

	return d;
}

bool GameObject::IsSphereCollider()
{
	return myCollider->isOf<Banana::SphereCollider>();
}

bool GameObject::IsBoxCollider()
{
	return myCollider->isOf<Banana::BoxCollider>();
}

void GameObject::RemoveCollider()
{
	delete myCollider;
	myCollider = nullptr;
}


