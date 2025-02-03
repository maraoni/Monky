#include "MonkyPhysics.h"
#include "MonkyEngine.h"
#include "GameObject.h"
#include "Collisions.h"
#include "Intersections.h"
#include <iostream>
#include "VirtualObject.h"
#include <gtc/matrix_transform.hpp>
#include "MonkeyMath.h"

namespace Banana
{
	MonkyPhysics::MonkyPhysics(Engine::MonkyEngine* aEngine)
	{
		myEngine = aEngine;
	}

	void MonkyPhysics::Simulate(const float& aDeltaTime)
	{
		std::vector<Collider*> cols = UpdatePhysicsScene();
		std::vector<Collision> collisions = CheckIntersections(cols);

		HandleCollisions(collisions);
		ApplyVelocity(cols, aDeltaTime);

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
		for (Collider* c1 : colliders)
		{
			for (Collider* c2 : colliders)
			{
				if (c1 != c2)
				{
					if (CheckIntersect(c1, c2))
					{
						std::cout << "Hey we are colliding" << std::endl;
						std::cout << "!" << std::endl;

						Collision collision;
						collision.col1 = c1;
						collision.col2 = c2;
						collisions.push_back(collision);
					}
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
}
