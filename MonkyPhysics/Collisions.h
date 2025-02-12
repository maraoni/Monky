#pragma once
#include <glm.hpp>
#include <iostream>

namespace Banana
{
	class Collider
	{
	public: virtual ~Collider() {}

		  template<typename T>
		  bool isOf() { return (dynamic_cast<T*>(this) != NULL); }

		  glm::vec3		center;
		  glm::vec3		position;
		  glm::mat4		transform;
		  //move to dynamic body
		  bool			hasGravity;
		  bool			isKinematic;
		  glm::vec3		velocity;
		  float			mass;
	};

	struct Collision
	{
		Collider* col1;
		Collider* col2;

		glm::vec3 point;
		glm::vec3 normal;
	};

	class SphereCollider : public Collider
	{
	public:
		SphereCollider(const glm::vec3& aCenter, const float& aRadius)
		{
			velocity = glm::vec3(0, 0, 0);
			hasGravity = false;
			center = aCenter;
			radius = aRadius;
			mass = 1;
		}
		float radius;
	};

	class BoxCollider : public Collider
	{
	public:
		BoxCollider(const glm::vec3& aCenter, const glm::vec3& someExtents)
		{
			velocity = glm::vec3(0, 0, 0);
			hasGravity = false;
			center = aCenter;
			extents = someExtents;
			mass = 1;
		}
		glm::vec3 extents;
	};

	class PlaneCollider : public Collider
	{
	public:
		PlaneCollider(const glm::vec3& aNormal, float aDistance)
		{
			velocity = glm::vec3(0, 0, 0);
			hasGravity = false;
			isKinematic = true;
			normal = glm::normalize(aNormal); 
			distance = aDistance;
			mass = 1; 
		}

		glm::vec3 normal; 
		float distance;   
	};
}