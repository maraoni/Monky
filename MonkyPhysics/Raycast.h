#pragma once
#include <glm.hpp>
#include <iostream>

namespace Banana
{
	class Collider;
}

class Ray
{
public:
	Ray(const glm::vec3& anOrigin, const glm::vec3& aDirection)
	{
		origin = anOrigin;
		direction = aDirection;
	}

	glm::vec3 origin;
	glm::vec3 direction;
};

struct RayHit
{
	RayHit() {};
	RayHit(glm::vec3 aPoint, Banana::Collider* aCollider, float aDistance)
	{
		point = aPoint;
		collider = aCollider;
		distance = aDistance;
	}

	glm::vec3 point;
	Banana::Collider* collider;
	float distance;

};