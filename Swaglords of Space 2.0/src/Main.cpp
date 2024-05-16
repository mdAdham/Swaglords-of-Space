#include <stdio.h>
#include <cstdlib>
#include <iostream>

#include "Managers_Impl/LogManager.hpp"
#include "Core/Log.hpp"

#include "Engine_2.0_Impl/Engine_2.0_Impl.h"

int main()
{
	//printf("Hello World!\n");
	std::srand(std::time(0));

	_Swag::LogManager::Initialize();

	_SWAG_DEBUGS("SwagSpace v{0}.{1}", 2, 0);

	_Swag::Engine_2::Engine_v2_Impl enginev2;

	enginev2.Initialize();

	enginev2.Run();

	enginev2.~Engine_v2_Impl();
		
	_Swag::LogManager::ShutDown();

#ifdef SWAG_DEBUG
	std::cin.get();
#endif // SWAG_DEBUG

	//delete game;
	return 0;
}