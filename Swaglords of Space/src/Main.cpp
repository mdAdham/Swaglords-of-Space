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

	Swag::Game game(SCREEN_HEIGHT, SCREEN_WIDTH, "SWAGLORDS OF SPACE", DEV_SCREEN, DEV_SCREEN + 50, "DEVELOPER WINDOW");

	SWAG_INFO("Game Destroyed");

	Swag::LogManager::ShutDown();

	std::cin.get();

	return	EXIT_SUCCESS;
}