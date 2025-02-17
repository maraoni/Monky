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
#include <gtx/string_cast.hpp>


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
		float limitDt = glm::min(aDeltaTime, 0.02f);

		colliders = UpdatePhysicsScene();
		std::vector<Collision> collisions = CheckIntersections(colliders);

		ApplyGravity(colliders, limitDt);

		HandleCollisions(collisions);

		ApplyVelocity(colliders, limitDt);

		UpdateVisuals();
	}

	void MonkyPhysics::ApplyGravity(std::vector<Collider*> colliders, const float& dt)
	{
		for (Collider* c : colliders)
		{
			if (!c->isKinematic && c->hasGravity)
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
				std::cout << glm::to_string(c->angularVelocity) << std::endl;
				c->position += c->velocity * dt;
				c->transform[3] = glm::vec4(c->position, 1.0f);

				if (glm::length(c->angularVelocity) > 0.0001f)
				{
					glm::vec3 angularVelocityNorm = glm::normalize(c->angularVelocity);
					glm::quat angularRotation = glm::angleAxis(glm::length(c->angularVelocity) * dt, angularVelocityNorm);
					glm::mat3 rotationDelta = glm::mat3_cast(angularRotation);
					c->transform = glm::mat4(rotationDelta) * c->transform;
				}

				if (c->mass > 0)
				{
					c->velocity *= glm::pow(1.0f - LinearDrag, dt);
				}
				if (c->mass > 0)
				{
					c->angularVelocity *= glm::pow(1.0f - AngularDrag, dt);
				}
			}
			else
			{
				c->velocity = glm::vec3(0, 0, 0);
				c->angularVelocity = glm::vec3(0, 0, 0);
			}
		}
	}

	void MonkyPhysics::HandleCollisions(std::vector<Collision> collisions)
	{

		std::vector<Collision> dynamicDynamicCollisions;
		std::vector<Collision> staticDynamicCollisions;

		for (Collision c : collisions)
		{
			bool A_isDynamic = !c.col1->isKinematic;
			bool B_isDynamic = !c.col2->isKinematic;

			if (A_isDynamic && B_isDynamic)
				dynamicDynamicCollisions.push_back(c);
			else
				staticDynamicCollisions.push_back(c);
		}

		HandleDynamicDynamic(dynamicDynamicCollisions);
		HandleStaticDynamic(staticDynamicCollisions);

	}

	void MonkyPhysics::HandleStaticDynamic(std::vector<Collision> collisions)
	{
		for (Collision c : collisions)
		{
			Collider* A = c.col1;
			Collider* B = c.col2;

			bool A_isDynamic = !A->isKinematic;
			bool B_isDynamic = !B->isKinematic;

			if (!A_isDynamic && !B_isDynamic) continue;

			Collider* dynamicCollider = A_isDynamic ? A : B;
			Collider* staticCollider = A_isDynamic ? B : A;

			glm::vec3 n = glm::normalize(c.normal);
			glm::vec3 r = c.point - dynamicCollider->position;

			glm::vec3 v = dynamicCollider->velocity + glm::cross(dynamicCollider->angularVelocity, r);
			float vRelDotN = glm::dot(v, n);

			if (vRelDotN > 0) continue;

			float invMass = (dynamicCollider->mass > 0) ? 1.0f / dynamicCollider->mass : 0;

			glm::vec3 r_cross_n = glm::cross(r, n);
			float angularEffect = glm::dot(r_cross_n, dynamicCollider->inverseMomentOfInertia * r_cross_n);

			float impulseMagnitude = -(1 + Restitution) * vRelDotN / (invMass + angularEffect);
			glm::vec3 impulse = impulseMagnitude * n;

			dynamicCollider->velocity += impulse * invMass;

			dynamicCollider->angularVelocity += dynamicCollider->inverseMomentOfInertia * glm::cross(r, impulse);
		}
	}

	void MonkyPhysics::HandleDynamicDynamic(std::vector<Collision> collisions)
	{
		for (Collision c : collisions)
		{
			glm::vec3 normal = c.normal;

			glm::vec3 relativeVelocity = c.col2->velocity - c.col1->velocity;
			float velocityAlongNormal = glm::dot(relativeVelocity, normal);

			if (velocityAlongNormal > 0) continue;

			float impulse = (1 + Restitution) * velocityAlongNormal;

			glm::vec3 impulseVector = impulse * normal;

			c.col1->velocity += impulseVector;
			c.col2->velocity -= impulseVector;

			glm::vec3 r1 = c.point - c.col1->position;
			glm::vec3 r2 = c.point - c.col2->position;

			glm::vec3 torque1 = glm::cross(r1, impulseVector);
			glm::vec3 torque2 = glm::cross(r2, impulseVector);

			// Rotate! sort of....
			c.col1->angularVelocity += c.col1->inverseMomentOfInertia * torque1 * 100000000.0f;
			c.col2->angularVelocity -= c.col2->inverseMomentOfInertia * torque2 * 100000000.0f;
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
				glm::mat4 trans = c->GetVirtual()->GetTrans();

				col->transform = trans;
				col->position = glm::vec3(trans[3]);

				glm::mat3 rotationMatrix = glm::mat3(col->transform);
				glm::mat3 inertiaTensorInWorldSpace = rotationMatrix * col->momentOfInertia * glm::transpose(rotationMatrix);

				col->inverseMomentOfInertia = inertiaTensorInWorldSpace;

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
