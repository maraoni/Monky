#pragma once

class VirtualObject;
namespace Chimp
{
	class ObjectEntry
	{
	public:

		ObjectEntry(VirtualObject* anObject);
		~ObjectEntry();

		void Update();

		bool Opened = false;

		VirtualObject* myObject;
	private:
	};
}
