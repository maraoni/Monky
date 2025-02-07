#include <glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/norm.hpp>
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
		else if (aCollider1->isOf<BoxCollider>() && aCollider2->isOf<SphereCollider>())
		{
			BoxCollider* box1 = dynamic_cast<BoxCollider*>(aCollider1);
			SphereCollider* sphere2 = dynamic_cast<SphereCollider*>(aCollider2);
			return BoxSphereIntersect(*box1, *sphere2);
		}
		else if (aCollider1->isOf<BoxCollider>() && aCollider2->isOf<BoxCollider>())
		{
			BoxCollider* box1 = dynamic_cast<BoxCollider*>(aCollider1);
			BoxCollider* box2 = dynamic_cast<BoxCollider*>(aCollider2);
			return BoxBoxIntersect(*box1, *box2);
		}
	}

	bool CheckRayIntersect(const Ray& aRay, Collider* aCollider)
	{
		if (aCollider->isOf<SphereCollider>())
		{
			SphereCollider* sphere = dynamic_cast<SphereCollider*>(aCollider);
			return RaySphereIntersect(aRay, *sphere);
		}
		else if (aCollider->isOf<BoxCollider>())
		{
			BoxCollider* box = dynamic_cast<BoxCollider*>(aCollider);
			return RayBoxIntersect(aRay, *box);
		}
		return false;
	}

	bool RaySphereIntersect(const Ray& aRay, const SphereCollider& aSphere)
	{
		glm::vec3 sphereCenter = glm::vec3(aSphere.transform[3]);
		float radius = aSphere.radius;

		glm::vec3 oc = aRay.origin - sphereCenter;
		float a = glm::dot(aRay.direction, aRay.direction);
		float b = 2.0f * glm::dot(oc, aRay.direction);
		float c = glm::dot(oc, oc) - radius * radius;

		float discriminant = b * b - 4 * a * c;
		if (discriminant < 0) return false;

		float t = (-b - sqrt(discriminant)) / (2.0f * a);
		return t >= 0;
	}

	bool RayBoxIntersect(const Ray& aRay, const BoxCollider& aBox)
	{
		glm::vec3 min = glm::vec3(aBox.transform[3]) - aBox.extents;
		glm::vec3 max = glm::vec3(aBox.transform[3]) + aBox.extents;

		glm::vec3 invDir = 1.0f / aRay.direction;

		float t1 = (min.x - aRay.origin.x) * invDir.x;
		float t2 = (max.x - aRay.origin.x) * invDir.x;
		float t3 = (min.y - aRay.origin.y) * invDir.y;
		float t4 = (max.y - aRay.origin.y) * invDir.y;
		float t5 = (min.z - aRay.origin.z) * invDir.z;
		float t6 = (max.z - aRay.origin.z) * invDir.z;

		float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
		float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

		return tmax >= std::max(0.0f, tmin);
	}

	bool RayOBBIntersect(const Ray& aRay, const BoxCollider& aBox)
	{
		glm::vec3 center = glm::vec3(aBox.transform[3]);
		glm::mat3 rotation = glm::mat3(aBox.transform);

		glm::vec3 localOrigin = glm::transpose(rotation) * (aRay.origin - center);
		glm::vec3 localDirection = glm::transpose(rotation) * aRay.direction;

		BoxCollider localBox = BoxCollider(glm::vec3(0, 0, 0), aBox.extents);
		localBox.extents = aBox.extents;

		Ray localRay(localOrigin, localDirection);
		return RayBoxIntersect(localRay, localBox);
	}

	bool SphereSphereIntersect(const SphereCollider& aSphere1, const SphereCollider& aSphere2)
	{
		glm::vec3 pos1 = glm::vec3(aSphere1.transform[3]);
		glm::vec3 pos2 = glm::vec3(aSphere2.transform[3]);
		float dist2 = glm::distance2(pos1, pos2);
		float radiusSum = aSphere1.radius + aSphere2.radius;
		return dist2 < radiusSum * radiusSum;
	}

	bool BoxBoxIntersect(const BoxCollider& aBox1, const BoxCollider& aBox2)
	{
		glm::mat3 rotation1 = glm::mat3(aBox1.transform);
		glm::mat3 rotation2 = glm::mat3(aBox2.transform);
		glm::vec3 translation = glm::vec3(aBox2.transform[3]) - glm::vec3(aBox1.transform[3]);
		translation = glm::transpose(rotation1) * translation;

		glm::mat3 rotation = glm::transpose(rotation1) * rotation2;

		glm::mat3 absRotation = glm::mat3(
			glm::abs(rotation[0]),
			glm::abs(rotation[1]),
			glm::abs(rotation[2])
		) + glm::mat3(0.000001f);

		// SAT (seperating axis theorem)
		for (int i = 0; i < 3; ++i) {
			float ra = aBox1.extents[i];
			float rb = glm::dot(absRotation[i], aBox2.extents);
			if (glm::abs(translation[i]) > ra + rb) return false;
		}

		for (int i = 0; i < 3; ++i) {
			float ra = glm::dot(absRotation[i], aBox1.extents);
			float rb = aBox2.extents[i];
			if (glm::abs(glm::dot(rotation[i], translation)) > ra + rb) return false;
		}

		return true;
	}

	bool BoxSphereIntersect(const BoxCollider& aBox1, const SphereCollider& aSphere2)
	{
		glm::vec3 sphereCenter = glm::vec3(aSphere2.transform[3]);
		glm::vec3 localSphereCenter = glm::inverse(aBox1.transform) * glm::vec4(sphereCenter, 1.0f);
		glm::vec3 closestPoint = glm::clamp(localSphereCenter, -aBox1.extents, aBox1.extents);
		float dist2 = glm::length2(localSphereCenter - closestPoint);
		return dist2 < aSphere2.radius * aSphere2.radius;
	}
}

