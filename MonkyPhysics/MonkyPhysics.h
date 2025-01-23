#pragma once

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
	class MonkyPhysics
	{
	public:
		MonkyPhysics(Engine::MonkyEngine* aEngine);
		void Simulate();

	private:

		Engine::MonkyEngine* myEngine;
	};
}

