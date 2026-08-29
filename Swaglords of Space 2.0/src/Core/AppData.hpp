#pragma once

#include <State_Impl/StateMachine.hpp>
#include <Managers_Impl/AssetManager.hpp>
#include <Managers_Impl/AudioManager.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Core.hpp"

namespace _Swag {

	struct GameData
	{
		_StateMachine machine;
		sf::RenderWindow window;
		_AssetManager assets;
		AudioManager audio;
		bool quit = false;
	public:
		GameData()
			:audio(assets, 32)
		{

		}
	};
}