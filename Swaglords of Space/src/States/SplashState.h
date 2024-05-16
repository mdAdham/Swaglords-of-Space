#pragma once

#include <SFML/Graphics.hpp>
#include "State/State.h"
#include "Game.h"

namespace Swag
{
	class SplashState : public State
	{
	public:
		SplashState(GameDataRef data);

		void Init();

		void HandleInput();
		void Update(float dt);
		void Draw(float dt);

	private:
		GameDataRef _data;

		sf::Clock _clock;

		sf::Sprite _background;

		sf::Text loadingText;

	};
}