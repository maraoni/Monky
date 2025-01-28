#pragma once

#include <glm.hpp>

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
		void Simulate(const float& aDeltaTime);



	private:

		Engine::MonkyEngine* myEngine;
	};
}
//
//template <class DstType, class SrcType>
//bool IsType(const SrcType* src)
//{
//	return dynamic_cast<const DstType*>(src) != nullptr;
//}

