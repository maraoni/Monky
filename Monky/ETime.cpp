#include "ETime.h"

void Engine::ETime::UpdateDeltaTime(const float& aDeltaTime)
{
	myDeltaTime = aDeltaTime;
}

const float& Engine::ETime::DeltaTime()
{
	return myDeltaTime;
}
