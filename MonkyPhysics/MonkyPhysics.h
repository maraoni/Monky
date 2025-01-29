#pragma once

#include <glm.hpp>
#include <vector>

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

		void BeginStep(std::vector<Collider*> colliders, const float& dt);
		void HandleCollisions(std::vector<Collision> collisions);
		void EndStep();

	private:

		Engine::MonkyEngine* myEngine;
	};
}

