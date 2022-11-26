#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "State/State.h"
#include "States/LevelState.h"
#include "Game.h"

namespace Swag
{
	class MainMenuState : public State
	{
	public:

		MainMenuState(GameDataRef data);

		void Init();

		void HandleInput();
		void Update(float dt);
		void Draw(float dt);

	private:
		GameDataRef _data;

		sf::Sprite _background;

		sf::Sprite _playButton;

		sf::Text _quitButton;

		sf::Text _optionButton;

		///////////////////////////////////
		sf::SoundBuffer musicBuff;
		sf::Sound music;
		short int music_count = 0;
		///////////////////////////////////

	private:
		enum class ButtonStatus
		{
			idle,
			hover,
			click
		};

		ButtonStatus optionButtonStatus;
		ButtonStatus quitButtonStatus;
		ButtonStatus playButtonStatus;
	};
}

