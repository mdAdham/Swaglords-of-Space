#pragma once

#include <SFML/Graphics.hpp>

#include "Game.h"
#include "State/State.h"

namespace Swag
{
	class PauseState : public State
	{
	public:
		PauseState(GameDataRef data);

		void Init();

		void HandleInput();
		void Update(float dt);
		void Draw(float dt);

	private:
		GameDataRef _data;

		sf::Sprite _background;

		sf::Sprite _pauseButton;
		sf::Sprite _homeButton;

	private:
		enum class ButtonStatus
		{
			idle,
			hover,
			click
		};

		ButtonStatus pauseButtonStatus;
		ButtonStatus homeButtonStatus;
	};
}