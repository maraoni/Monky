#include "MonkyPhysics.h"
#include "MonkyEngine.h"
#include <iostream>

namespace Banana
{
	MonkyPhysics::MonkyPhysics(Engine::MonkyEngine* aEngine)
	{
		myEngine = aEngine;
	}

	void MonkyPhysics::Simulate()
	{
		std::cout << "Simulating" << std::endl;
	}
}
