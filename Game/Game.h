#pragma once

namespace Engine
{
	class Input;
};

class Game
{
public:
	Game(Engine::Input* aInputSystem);
	void Update(const float& aDeltaTime);

private:
	Engine::Input* myInput;
};

