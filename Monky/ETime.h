#pragma once
namespace Engine
{
	class ETime
	{
	public:

		void UpdateDeltaTime(const float& aDeltaTime);
		const float& DeltaTime();

	private:

		float myDeltaTime;
	};
}

