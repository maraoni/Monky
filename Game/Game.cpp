#include "Game.h"
#include "Input.h"
#include <iostream>

int main()
{
	return 0;
}

Game::Game(Engine::Input* aInputSystem)
{
	myInput = aInputSystem;
}

void Game::Update(const float& aDeltaTime)
{
	if (myInput->IsKeyDown(87))
	{
		std::cout << aDeltaTime << std::endl;
	}
}
