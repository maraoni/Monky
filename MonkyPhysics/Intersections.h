#pragma once
#include "Collisions.h"
#include "Raycast.h"

namespace Banana
{
	// Colliders
	Collision CheckIntersect(Collider* aCollider1, Collider* aCollider2);

	Collision SphereSphereIntersect(const SphereCollider& aSphere1, const SphereCollider& aSphere2);
	Collision BoxBoxIntersect(const BoxCollider& aBox1, const BoxCollider& aBox2);
	Collision BoxSphereIntersect(const BoxCollider& aBox1, const SphereCollider& aSphere2);
	Collision PlaneBoxIntersect(const PlaneCollider& aPlane, const BoxCollider& aBox);
	Collision PlaneSphereIntersect(const PlaneCollider& aPlane, const SphereCollider& aSphere);

	// Rays
	bool CheckRayIntersect(const Ray& aRay, Collider* aCollider);

	bool RaySphereIntersect(const Ray& aRay, const SphereCollider& aSphere);
	bool RayBoxIntersect(const Ray& aRay,	const BoxCollider& aBox);
	bool RayOBBIntersect(const Ray& aRay, const BoxCollider& aBox);
}