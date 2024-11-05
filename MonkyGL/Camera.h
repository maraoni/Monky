#pragma once
#include <glm.hpp>
#include <glfw3.h>
namespace Gorilla
{

	class Camera
	{
	private:

		glm::vec3 myPosition;
		glm::vec3 myDirection;
		glm::vec3 myTarget;

		glm::vec3 myUp;
		glm::vec3 myRight;

	public:

		glm::mat4 myView;

		Camera();

		void CameraUpdate();

		void SetPosition(const glm::vec3& aPosition);
		void SetRotation(const glm::vec3& aRotation);

		void Move(const glm::vec3& aMove);
		void SetDirection(const glm::vec3& aDirection);
	};
}

