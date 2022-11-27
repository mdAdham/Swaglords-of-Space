#pragma once

#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include "State/StateMachine.h"
#include "Manager/AssetManager.h"
#include "Manager/InputManager.h"

//#include "Manager/logManager.h"
#include "Core/log.h"

namespace Swag
{
	struct GameData
	{
		StateMachine machine;
		sf::RenderWindow window;
		AssetManager assets;
		InputManager input;
	};

	typedef std::shared_ptr<GameData> GameDataRef;

	class Game
	{
	public:
		Game(int width, int height, std::string title, int DevWin_height, int DevWin_width, std::string DevWin_Title);

	private:
		const float dt = 1.0f / 144.0f;
		sf::Clock _clock;


		GameDataRef _data = std::make_shared<GameData>();
		GameDataRef _data2 = std::make_shared<GameData>();

		void Run();
	};
}