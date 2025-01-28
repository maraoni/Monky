#pragma once
#include <glm.hpp>


namespace Banana
{
	class Collider
	{
	public: virtual ~Collider() {}

		  glm::vec3 position;
		  template<typename T>
		  bool isOf() {
			  return (dynamic_cast<T*>(this) != NULL);
		  }

		  //move to dynamic body
		  bool HasGravity;
		  glm::vec3 velocity;

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

	class SphereCollider : public Collider
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

	class BoxCollider : public Collider
	{
	public:
		BoxCollider(const glm::vec3& aCenter, const glm::vec3& someExtents)
		{
			center = aCenter;
			extents = someExtents;
		}
	protected:
		glm::vec3 extents;
	};
}