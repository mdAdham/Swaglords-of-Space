#pragma once

#include <SFML/Graphics.hpp>

#include "State/State.h"
#include "Game.h"

namespace Swag
{
	class OptionState : public State
	{
	public:

		OptionState(GameDataRef data);

		void Init();

		void HandleInput();
		void Update(float dt);
		void Draw(float dt);

	private:
		GameDataRef _data;

		sf::Text MusicONOFF;
		bool isPressed = false;
	private:
		void initText();

	};
}