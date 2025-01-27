#pragma once

class GameObject;
namespace Chimp
{
	class ObjectEntry
	{
	public:

		ObjectEntry(GameObject* anObject);
		~ObjectEntry();

		void Update();

		bool Opened = false;

		GameObject* myObject;
	private:
	};
}
