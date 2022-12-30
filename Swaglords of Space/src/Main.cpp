#include <iostream>
#include <cstdlib>
#include <string>

#include "Game.h"
#include "Core/DEFFINITIONS.h"

#include "Manager/logManager.h"
#include "Core/log.h"

int main()
{
	std::srand(std::time(0));

	Swag::LogManager::Initialize();

	SWAG_TRACE("SwagLord v{}.{}", 2, 20);

	Swag::Game* game = new Swag::Game(SCREEN_HEIGHT, SCREEN_WIDTH, "SWAGLORDS OF SPACE", DEV_SCREEN, DEV_SCREEN + 50, "DEVELOPER WINDOW", false);

	SWAG_INFO("Game Destroyed");

	Swag::LogManager::ShutDown();

	#ifdef SWAG_DEBUG
	std::cin.get();
	#endif // SWAG_DEBUG

	delete game;
	return	EXIT_SUCCESS;
}