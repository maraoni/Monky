#pragma once
#include "Collisions.h"
#include "Raycast.h"

namespace Banana
{
	// Colliders
	bool CheckIntersect(Collider* aCollider1, Collider* aCollider2);

	bool SphereSphereIntersect(const SphereCollider& aSphere1, const SphereCollider& aSphere2);
	bool BoxBoxIntersect(const BoxCollider& aBox1, const BoxCollider& aBox2);
	bool BoxSphereIntersect(const BoxCollider& aBox1, const SphereCollider& aSphere2);

	// Rays
	bool CheckRayIntersect(const Ray& aRay, Collider* aCollider);

	bool RaySphereIntersect(const Ray& aRay, const SphereCollider& aSphere);
	bool RayBoxIntersect(const Ray& aRay,	const BoxCollider& aBox);
	bool RayOBBIntersect(const Ray& aRay, const BoxCollider& aBox);
}