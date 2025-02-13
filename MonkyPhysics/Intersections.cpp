#include <glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/norm.hpp>
#include "Collisions.h"
#include "Intersections.h"

namespace Banana
{
	Collision CheckIntersect(Collider* aCollider1, Collider* aCollider2)
	{
		if (aCollider1->isOf<BoxCollider>() && aCollider2->isOf<SphereCollider>())		std::swap(aCollider1, aCollider2);
		if (aCollider1->isOf<BoxCollider>() && aCollider2->isOf<PlaneCollider>())		std::swap(aCollider1, aCollider2);
		if (aCollider1->isOf<SphereCollider>() && aCollider2->isOf<PlaneCollider>())	std::swap(aCollider1, aCollider2);

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
		else if (aCollider1->isOf<PlaneCollider>() && aCollider2->isOf<BoxCollider>())
		{
			PlaneCollider* plane = dynamic_cast<PlaneCollider*>(aCollider1);
			BoxCollider* box = dynamic_cast<BoxCollider*>(aCollider2);
			return PlaneBoxIntersect(*plane, *box);
		}
		else if (aCollider1->isOf<PlaneCollider>() && aCollider2->isOf<SphereCollider>())
		{
			PlaneCollider* plane = dynamic_cast<PlaneCollider*>(aCollider1);
			SphereCollider* sphere = dynamic_cast<SphereCollider*>(aCollider2);
			return PlaneSphereIntersect(*plane, *sphere);
		}
		return { nullptr, nullptr, glm::vec3() };
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
		//Look at glm "intersectRaySphere"

		// vector from ray origin to sphere center
		glm::vec3 center = aSphere.transform[3];
		glm::vec3 diff = center - aRay.origin;

		// project diff onto ray direction
		float t0 = glm::dot(diff, aRay.direction);

		// perpendicular distance! ....? also dot on itself makes sense since diff is not normalized
		float dSquared = glm::dot(diff, diff) - t0 * t0;

		// distance is greater than the sphere's radius squared, no intersection
		float radiusSquared = aSphere.radius * aSphere.radius;
		if (dSquared > radiusSquared)
		{
			return false;
		}

		// distance from closest to the intersection point
		float t1 = glm::sqrt(radiusSquared - dSquared);

		float Epsilon = 0.000001f; // just a small number to help with tiny floating point errors
		float outIntersectionDistance = (t0 > t1 + Epsilon) ? t0 - t1 : t0 + t1; // could actually return this as the distance

		// return true if intersection distance is positive
		return outIntersectionDistance > Epsilon;
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

	Collision SphereSphereIntersect(const SphereCollider& aSphere1, const SphereCollider& aSphere2)
	{
		Collision returnCollision = { nullptr, nullptr, glm::vec3() };

		glm::vec3 pos1 = glm::vec3(aSphere1.transform[3]);
		glm::vec3 pos2 = glm::vec3(aSphere2.transform[3]);
		float dist2 = glm::distance2(pos1, pos2);
		float radiusSum = aSphere1.radius + aSphere2.radius;

		if (dist2 < radiusSum * radiusSum)
		{
			float dist = glm::sqrt(dist2);
			glm::vec3 normal = (dist > 0.0f) ? (pos2 - pos1) / dist : glm::vec3(1, 0, 0);
			glm::vec3 contactPoint = pos1 + normal * aSphere1.radius;

			returnCollision = { const_cast<SphereCollider*>(&aSphere1), const_cast<SphereCollider*>(&aSphere2), contactPoint, normal };
		}

		return returnCollision;
	}

	Collision BoxBoxIntersect(const BoxCollider& aBox1, const BoxCollider& aBox2)
	{
		Collision returnCollision = { nullptr, nullptr, glm::vec3(), glm::vec3() };

		// Extract rotation and translation
		glm::mat3 rotation1 = glm::mat3(aBox1.transform);
		glm::mat3 rotation2 = glm::mat3(aBox2.transform);
		glm::vec3 translation = glm::vec3(aBox2.transform[3]) - glm::vec3(aBox1.transform[3]);

		glm::mat3 rotation = rotation1 * glm::transpose(rotation2);
		glm::mat3 absRotation = glm::mat3(
			glm::abs(rotation[0]) + 0.000001f,
			glm::abs(rotation[1]) + 0.000001f,
			glm::abs(rotation[2]) + 0.000001f
		);

		float minPenetration = FLT_MAX;
		glm::vec3 bestAxis = glm::vec3(0.0f);

		// --- 1. Box 1's face axes ---
		for (int i = 0; i < 3; ++i) {
			float ra = aBox1.extents[i];
			float rb = glm::dot(absRotation[i], aBox2.extents);
			float penetration = (ra + rb) - glm::abs(glm::dot(translation, rotation1[i]));

			if (penetration < 0.0f) return returnCollision;

			if (penetration < minPenetration) {
				minPenetration = penetration;
				bestAxis = rotation1[i];
			}
		}

		// --- 2. Box 2's face axes ---
		for (int i = 0; i < 3; ++i) {
			float ra = glm::dot(absRotation[i], aBox1.extents);
			float rb = aBox2.extents[i];
			float penetration = (ra + rb) - glm::abs(glm::dot(translation, rotation2[i]));

			if (penetration < 0.0f) return returnCollision;

			if (penetration < minPenetration) {
				minPenetration = penetration;
				bestAxis = rotation2[i];
			}
		}

		// --- 3. Cross-product axes ---
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				glm::vec3 axis = glm::cross(rotation1[i], rotation2[j]);

				if (glm::length(axis) < 0.0001f) continue;
				axis = glm::normalize(axis);

				float ra = glm::dot(aBox1.extents, glm::abs(rotation1 * axis));
				float rb = glm::dot(aBox2.extents, glm::abs(rotation2 * axis));
				float penetration = (ra + rb) - glm::abs(glm::dot(translation, axis));

				if (penetration < 0.0f) return returnCollision;

				if (penetration < minPenetration) {
					minPenetration = penetration;
					bestAxis = axis;
				}
			}
		}

		// Ensure normal points from box1 to box2
		glm::vec3 normal = glm::normalize(bestAxis);
		if (glm::dot(normal, translation) < 0.0f) {
			normal = -normal;
		}

		// Compute contact point
		glm::vec3 contactPoint = glm::vec3(aBox1.transform[3]) + normal * minPenetration * 0.5f;

		return { const_cast<BoxCollider*>(&aBox1), const_cast<BoxCollider*>(&aBox2), contactPoint, normal };
	}

	Collision BoxSphereIntersect(const BoxCollider& aBox1, const SphereCollider& aSphere2)
	{
		glm::vec3 sphereCenter = glm::vec3(aSphere2.transform[3]);
		glm::vec3 localSphereCenter = glm::inverse(aBox1.transform) * glm::vec4(sphereCenter, 1.0f);
		glm::vec3 closestPoint = glm::clamp(localSphereCenter, -aBox1.extents, aBox1.extents);
		float dist2 = glm::length2(localSphereCenter - closestPoint);

		if (dist2 < aSphere2.radius * aSphere2.radius)
		{
			glm::vec3 normal = glm::normalize(localSphereCenter - closestPoint);
			glm::vec3 worldContactPoint = aBox1.transform * glm::vec4(closestPoint, 1.0f);
			return { const_cast<BoxCollider*>(&aBox1), const_cast<SphereCollider*>(&aSphere2), worldContactPoint, normal };
		}

		return { nullptr, nullptr, glm::vec3(0) };
	}

	Collision PlaneBoxIntersect(const PlaneCollider& aPlane, const BoxCollider& aBox)
	{
		Collision returnCollision = { nullptr, nullptr, glm::vec3(), glm::vec3() };
		
		glm::vec3 boxPos = glm::vec3(aBox.transform[3]);
		glm::mat3 rotation = glm::mat3(aBox.transform);
		glm::vec3 normal = glm::normalize(aPlane.normal);
		
		glm::vec3 corners[8];
		for (int i = 0; i < 8; ++i)
		{
			glm::vec3 offset = glm::vec3(
				(i & 1 ? 1 : -1) * aBox.extents.x,
				(i & 2 ? 1 : -1) * aBox.extents.y,
				(i & 4 ? 1 : -1) * aBox.extents.z
			);
			corners[i] = boxPos + rotation * offset;
		}
		
		int inFront = 0, behind = 0;
		for (const auto& corner : corners)
		{
			float dist = glm::dot(normal, corner) - aPlane.distance;
			if (dist > 0) inFront++;
			else behind++;
		}
		
		if (inFront > 0 && behind > 0)
		{
			glm::vec3 contactPoint = boxPos - normal * (glm::dot(normal, boxPos) - aPlane.distance);
			return { const_cast<PlaneCollider*>(&aPlane), const_cast<BoxCollider*>(&aBox), contactPoint, normal };
		}
		
		return returnCollision;
	}

	Collision PlaneSphereIntersect(const PlaneCollider& aPlane, const SphereCollider& aSphere)
	{

		glm::vec3 normal = glm::normalize(aPlane.normal);
		float d = glm::dot(normal, aPlane.position);
		float distance = glm::dot(normal, aSphere.position) - d;
		
		if (glm::abs(distance) <= aSphere.radius)
		{
			glm::vec3 collisionPoint = aSphere.position - normal * distance;
			return { const_cast<PlaneCollider*>(&aPlane), const_cast<SphereCollider*>(&aSphere), collisionPoint, normal };
		}
		
		return { nullptr, nullptr, glm::vec3(), glm::vec3() };
	}
}

