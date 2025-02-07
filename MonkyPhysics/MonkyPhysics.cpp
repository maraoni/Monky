#include "MonkyPhysics.h"
#include "MonkyEngine.h"
#include "GameObject.h"
#include "Collisions.h"
#include "Intersections.h"
#include <iostream>
#include "VirtualObject.h"
#include <gtc/matrix_transform.hpp>
#include "MonkeyMath.h"
#include "Raycast.h"


namespace Banana
{
	MonkyPhysics::MonkyPhysics(Engine::MonkyEngine* aEngine)
	{
		myEngine = aEngine;
	}

	void MonkyPhysics::Simulate(const float& aDeltaTime)
	{
		colliders = UpdatePhysicsScene();
		std::vector<Collision> collisions = CheckIntersections(colliders);

		HandleCollisions(collisions);
		ApplyVelocity(colliders, aDeltaTime);

		UpdateVisuals();
	}

	void MonkyPhysics::ApplyVelocity(std::vector<Collider*> colliders, const float& dt)
	{
		for (Collider* c : colliders)
		{
			if (c->hasGravity)
			{
				c->velocity += glm::vec3(0, GravityMultiplier, 0) * dt;
			}

			// Extract position from transform, apply velocity, and update transform
			glm::vec3 position = glm::vec3(c->transform[3]);
			position += c->velocity * dt;
			c->transform[3] = glm::vec4(position, 1.0f);
		}
	}

	void MonkyPhysics::HandleCollisions(std::vector<Collision> collisions)
	{
		for (Collision c : collisions)
		{

		}
	}

	std::vector<Collider*> MonkyPhysics::UpdatePhysicsScene()
	{
		std::vector<Collider*> cols;
		for (GameObject* c : myEngine->GetGameObjects())
		{
			Collider* col = c->GetCollider();
			if (col != nullptr)
			{
				col->transform = c->GetVirtual()->GetTrans();
				col->position = c->GetVirtual()->Position;
				cols.push_back(col);
			}
		}
		return cols;
	}

	std::vector<Collision> MonkyPhysics::CheckIntersections(std::vector<Collider*> colliders)
	{
		std::vector<Collision> collisions;

		int count = colliders.size();

		for (int i = 0; i < count; i++)
		{
			for (int j = i + 1; j < count; j++)
			{
				if (CheckIntersect(colliders[i], colliders[j]))
				{
					Collision collision;

					collision.col1 = colliders[i];
					collision.col2 = colliders[j];

					collisions.push_back(collision);
				}
			}
		}
		return collisions;
	}

	void MonkyPhysics::UpdateVisuals()
	{
		for (GameObject* c : myEngine->GetGameObjects())
		{
			Collider* col = c->GetCollider();
			if (col != nullptr)
			{
				c->GetVirtual()->SetTransform(col->transform);
			}
		}
	}

	bool MonkyPhysics::MonkeyRaycast(const Ray& aRay, RayHit& aHit)
	{
		for (Collider* c : colliders)
		{
			if (CheckRayIntersect(aRay, c))
			{
				aHit.collider = c;
				aHit.point = glm::vec3(0, 0, 0);
				aHit.distance = 10;
				return true;
			}
		}
		return false;
	}
}
