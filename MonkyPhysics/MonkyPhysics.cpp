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
				// Update position
				glm::vec3 position = glm::vec3(c->transform[3]);
				position += c->velocity * dt;
				c->position = position;
				c->transform[3] = glm::vec4(position, 1.0f);

				float angularDamping = 0.25f;

				c->angularVelocity *= angularDamping;

				// Update rotation using angular velocity (convert axis-angle to quaternion)
				float angle = glm::length(c->angularVelocity) * dt;
				if (angle > 0.0001f) // Prevent precision errors
				{
					glm::vec3 axis = glm::normalize(c->angularVelocity);
					glm::quat rotationQuat = glm::angleAxis(angle, axis);

					// Apply rotation to the current transform
					glm::mat3 rotationMatrix = glm::mat3(c->transform);
					rotationMatrix = glm::mat3(rotationQuat) * rotationMatrix;
					c->transform = glm::mat4(rotationMatrix);
					c->transform[3] = glm::vec4(position, 1.0f); // Keep position unchanged
				}
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
				glm::vec3 r1 = c.point - c.col1->position; // Offset from center of mass
				glm::vec3 r2 = c.point - c.col2->position;

				// Relative velocity
				glm::vec3 relativeVelocity = c.col2->velocity - c.col1->velocity;
				float velocityAlongNormal = glm::dot(relativeVelocity, normal);

				float impulse = 0;
				float invMass1 = 0;
				float invMass2 = 0;
				if (velocityAlongNormal < 0)
				{
					float restitution = 0.2f;
					invMass1 = c.col1->isKinematic ? 0.0f : (1.0f / c.col1->mass);
					invMass2 = c.col2->isKinematic ? 0.0f : (1.0f / c.col2->mass);

					// Compute rotational effects
					glm::vec3 cross1 = glm::cross(r1, normal);
					glm::vec3 cross2 = glm::cross(r2, normal);
					float angularEffect1 = invMass1 + glm::dot(cross1, cross1) / c.col1->mass;
					float angularEffect2 = invMass2 + glm::dot(cross2, cross2) / c.col2->mass;

					// Compute final impulse
					float impulseDenom = invMass1 + invMass2 + angularEffect1 + angularEffect2;
					impulse = (-(1 + restitution) * velocityAlongNormal) / impulseDenom;
					glm::vec3 impulseVector = impulse * normal;

					if (!c.col1->isKinematic)
					{
						c.col1->velocity -= impulseVector;
						c.col1->angularVelocity -= glm::cross(r1, impulseVector) * (1.0f / c.col1->momentOfInertia);
					}

					if (!c.col2->isKinematic)
					{
						c.col2->velocity += impulseVector;
						c.col2->angularVelocity += glm::cross(r2, impulseVector) * (1.0f / c.col2->momentOfInertia);
					}
				}

				// --- Friction ---
				glm::vec3 tangent = relativeVelocity - (glm::dot(relativeVelocity, normal) * normal);

				if (glm::length(tangent) > 0.0001f)
				{
					tangent = glm::normalize(tangent);

					float staticFriction = 0.2f;
					float dynamicFriction = 0.1f;
					float frictionImpulse = -glm::dot(relativeVelocity, tangent);

					float maxFriction = staticFriction * glm::abs(impulse) / (invMass1 + invMass2);
					if (glm::abs(frictionImpulse) > maxFriction)
					{
						frictionImpulse *= dynamicFriction;
					}

					glm::vec3 frictionVector = frictionImpulse * tangent;

					if (!c.col1->isKinematic)
					{
						c.col1->velocity -= frictionVector;
						c.col1->angularVelocity -= glm::cross(r1, frictionVector) * (1.0f / c.col1->momentOfInertia);
					}

					if (!c.col2->isKinematic)
					{
						c.col2->velocity += frictionVector;
						c.col2->angularVelocity += glm::cross(r2, frictionVector) * (1.0f / c.col2->momentOfInertia);
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
