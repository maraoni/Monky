#include <glm.hpp>
#include "Collisions.h"
#include "Intersections.h"

namespace Banana 
{
	bool CheckIntersect(Collider* aCollider1, Collider* aCollider2)
	{
		if (aCollider1->isOf<SphereCollider>() && aCollider2->isOf<SphereCollider>())
		{
			SphereCollider* sphere1 = dynamic_cast<SphereCollider*>(aCollider1);
			SphereCollider* sphere2 = dynamic_cast<SphereCollider*>(aCollider2);
			return SphereSphereIntersect(*sphere1, *sphere2);
		}
	}

	bool SphereSphereIntersect(const SphereCollider& aSphere1, const SphereCollider& aSphere2)
	{
		float dist = glm::distance(aSphere1.position, aSphere2.position);

		if (dist < aSphere1.radius + aSphere2.radius)
		{
			std::cout << "Spheres are intersecting!" << std::endl;
			return true;
		}
		else
		{
			return false;
		}
	}

}