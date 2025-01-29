#pragma once
#include "Collisions.h"

namespace Banana 
{
	bool SphereSphereIntersect(const SphereCollider& aSphere1, const SphereCollider& aSphere2);
	bool CheckIntersect(Collider* aCollider1, Collider* aCollider2);
}