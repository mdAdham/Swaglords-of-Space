#pragma once

#include <State_Impl/StateMachine.hpp>
#include <Managers_Impl/AssetManager.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Core.hpp"

namespace _Swag {

	struct GameData
	{
		_StateMachine machine;
		sf::RenderWindow window;
		_AssetManager assets;
		bool quit = false;
	};
}