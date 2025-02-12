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

		main_plane = new PlaneCollider(glm::vec3(0, 1, 0), 1);
		main_plane->position = glm::vec3(0, 0, 0);
	}

	void MonkyPhysics::Simulate(const float& aDeltaTime)
	{
		colliders = UpdatePhysicsScene();
		std::vector<Collision> collisions = CheckIntersections(colliders);

		ApplyGravity(colliders, aDeltaTime);

		HandleCollisions(collisions);

		ApplyVelocity(colliders, aDeltaTime);

		UpdateVisuals();
	}

	void MonkyPhysics::ApplyGravity(std::vector<Collider*> colliders, const float& dt)
	{
		for (Collider* c : colliders)
		{
			if (c->hasGravity && !c->isKinematic)
			{
				c->velocity += glm::vec3(0, GravityMultiplier, 0) * dt;
			}
		}
	}

	void MonkyPhysics::ApplyVelocity(std::vector<Collider*> colliders, const float& dt)
	{
		for (Collider* c : colliders)
		{
			if (!c->isKinematic)
			{
				glm::vec3 position = glm::vec3(c->transform[3]);
				position += c->velocity * dt;
				c->position = position;
				c->transform[3] = glm::vec4(position, 1.0f);
			}
		}
	}

	void MonkyPhysics::HandleCollisions(std::vector<Collision> collisions)
	{
		for (Collision c : collisions)
		{
			if (!c.col1->isKinematic || !c.col2->isKinematic)
			{
				glm::vec3 normal = c.normal;

				// Relative velocity
				glm::vec3 relativeVelocity = c.col2->velocity - c.col1->velocity;
				float velocityAlongNormal = glm::dot(relativeVelocity, normal);

				// < 0 means they are moving towards eachother
				if (velocityAlongNormal < 0)
				{
					// Coefficient of restitution (bounciness) and calculating impulse
					float restitution = 0.2f;
					float impulse = (1 + restitution) * velocityAlongNormal;

					if (!c.col1->isKinematic)
					{
						glm::vec3 impulseVector = impulse * normal;
						c.col1->velocity += impulseVector;
					}

					if (!c.col2->isKinematic)
					{
						glm::vec3 impulseVector = impulse * normal;
						c.col2->velocity -= impulseVector; 
					}
				}
			}
		}
	}

	std::vector<Collider*> MonkyPhysics::UpdatePhysicsScene()
	{
		std::vector<Collider*> cols;

		cols.push_back(main_plane);

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
				Collision c = CheckIntersect(colliders[i], colliders[j]);
				if (c.col1 != nullptr && c.col2 != nullptr)
				{
					collisions.push_back(c);
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
