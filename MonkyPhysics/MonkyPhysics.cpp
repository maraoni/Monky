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
		//Just setting the positions of the colliders to match the visual
		std::vector<Collider*> cols = UpdatePhysicsScene();

		//Checking for any intersections and storing their data in a vector of collisions
		std::vector<Collision> collisions = CheckIntersections(cols);

		//As a result of those collisions, what should happen?
		HandleCollisions(collisions);

		//At the moment this is only applying gravity to my colliders since i have no
		//calculations for linear and angular velocity based on collisions.
		//This should ideally be in HandleCollisions
		ApplyVelocity(cols, aDeltaTime);

		//Making sure that the visuals of the colliders is aligned with the colliders
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
			c->position += c->velocity * dt;
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

						//This will eventually become the return value of check intersect
						Collision collision;
						collision.col1 = c1;
						collision.col2 = c2;
						collisions.push_back(collision);
						//---------------------------------------------------------------
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
				c->GetVirtual()->Position = col->position;
			}
		}
	}
}
