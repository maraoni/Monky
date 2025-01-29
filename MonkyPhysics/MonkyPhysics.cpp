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

		for (Collider* c1 : cols)
		{
			for (Collider* c2 : cols)
			{
				if (c1 != c2)
				{
					if (CheckIntersect(c1, c2))
					{

					}
				}
			}
		}
	}
}
