#include "MonkyPhysics.h"
#include "MonkyEngine.h"
#include "GameObject.h"
#include "Collisions.h"
#include <iostream>

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
				cols.push_back(col);
			}
		}

		for (Collider* c1 : cols)
		{
			for (Collider* c2 : cols)
			{
				if (c1 != c2)
				{
					if (c1->isOf<SphereCollider>() && c2->isOf<SphereCollider>())
					{
						std::cout << "We have 2 sphere colliders" << std::endl;
					}
				}
			}
		}
	}
}
