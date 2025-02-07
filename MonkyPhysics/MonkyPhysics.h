#pragma once

#include <glm.hpp>
#include <vector>
#include <optional>
#include "Raycast.h"

const float GravityMultiplier = -9.82f;

namespace Engine
{
	class Input;
	class MonkyEngine;
}

namespace Gorilla
{
	class Camera;
	class OpenGLFrameBuffer;
}
namespace Banana 
{
	class Collider;
	struct Collision;
}

namespace Banana
{
	class MonkyPhysics
	{
	public:
		MonkyPhysics(Engine::MonkyEngine* aEngine);
		void Simulate(const float& aDeltaTime);

		void ApplyVelocity(std::vector<Collider*> colliders, const float& dt);
		void HandleCollisions(std::vector<Collision> collisions);
		std::vector<Collider*> UpdatePhysicsScene();
		std::vector<Collision> CheckIntersections(std::vector<Collider*> colliders);
		void UpdateVisuals();

		bool MonkeyRaycast(const Ray& aRay, RayHit& aHit);

	private:
		std::vector<Collider*> colliders;
		Engine::MonkyEngine* myEngine;
	};
}

