#include "MonkyPhysics.h"
#include "MonkyEngine.h"
#include "GameObject.h"
#include "Collisions.h"
#include "Intersections.h"
#include <iostream>
#include "VirtualObject.h"

namespace Banana
{
	MonkyPhysics::MonkyPhysics(Engine::MonkyEngine* aEngine)
	{
		myEngine = aEngine;
	}

	void MonkyPhysics::Simulate(const float& aDeltaTime)
	{
		std::vector<Collider*> cols;

		for (GameObject* c : myEngine->GetGameObjects())
		{
			Collider* col = c->GetCollider();
			if (col != nullptr)
			{
				col->position = c->GetVirtual()->Position;
				cols.push_back(col);
			}
		}

		BeginStep(cols, aDeltaTime);

		std::vector<Collision> collisions;
		for (Collider* c1 : cols)
		{
			for (Collider* c2 : cols)
			{
				if (c1 != c2)
				{
					if (CheckIntersect(c1, c2))
					{
						Collision collision;
						collision.col1 = c1;
						collision.col2 = c2;
					}
				}
			}
		}

		EndStep();
	}

	void MonkyPhysics::BeginStep(std::vector<Collider*> colliders, const float& dt)
	{
		for (Collider* c : colliders)
		{
			if (c->hasGravity)
			{
				c->velocity += glm::vec3(0, GravityMultiplier, 0) * dt;
			}
			c->position += c->velocity * dt;
		}
	}

	void MonkyPhysics::HandleCollisions(std::vector<Collision> collisions)
	{
		for (Collision c : collisions) 
		{

		}
	}

	void MonkyPhysics::EndStep()
	{
		for (GameObject* c : myEngine->GetGameObjects())
		{
			Collider* col = c->GetCollider();
			if (col != nullptr)
			{
				c->GetVirtual()->Position = col->position;
			}
		}
	}
}
