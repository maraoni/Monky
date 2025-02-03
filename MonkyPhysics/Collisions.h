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
		  glm::vec3		velocity;
		  float			mass;
	};

	struct Collision
	{
		Collider* col1;
		Collider* col2;

		glm::vec3 point;
		
		glm::vec3 normal1;
		glm::vec3 normal2;
	};

	struct CollisionResponse
	{

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
		}
		glm::vec3 extents;
	};

	class Ray 
	{
	public:
		Ray(const glm::vec3& anOrigin, const glm::vec3& aDirection) 
		{
			origin = anOrigin;
			direction = aDirection;
		}

	private:

		glm::vec3 origin;
		glm::vec3 direction;
	};

	struct RayHit
	{
		glm::vec3 point;
		float distance;

	};
}