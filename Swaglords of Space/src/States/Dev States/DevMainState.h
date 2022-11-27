#pragma once

#include <SFML/Graphics.hpp>
#include "State/State.h"
#include "Game.h"

namespace Swag
{
	class DevMainState : public State
	{
	public:
		DevMainState(GameDataRef data);

		void Init();

		void HandleInput();
		void Update(float dt);
		void Draw(float dt);

	private:
		GameDataRef _data;

		sf::Text _CreateNewGame;

	private:
		enum class ButtonStatus
		{
			idle,
			hover,
			click
		};

		ButtonStatus _newGameButtonStatus;

	private:
		void InitButtons();
		void InitText();

		void updateButtonStates(float dt);
	};
}