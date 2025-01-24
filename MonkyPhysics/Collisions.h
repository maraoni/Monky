#pragma once
#include <glm.hpp>


class Collider
{
public:
	Collider() = default;
	Collider(const glm::vec3& aCenter) 
	{
		center = aCenter;
	}

protected:
	glm::vec3 center;
};

struct Collision
{
	Collider* col1;
	Collider* col2;

	glm::vec3 point;
};

struct CollisionResponse
{

};

class SphereCollider : Collider
{
public:
	SphereCollider(const glm::vec3& aCenter, const float& aRadius)
	{
		center = aCenter;
		radius = aRadius;
	}
protected:
	float radius;
};
