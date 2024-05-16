#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "State/State.h"
#include "States/LevelState.h"
#include "Game.h"
#include "Core/Core.hpp"

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

		void DontDestroyOnLoad();

	private:
		GameDataRef _data;

		sf::Sprite _background;

		sf::Sprite _playButton;
		sf::Text _quitButton;
		sf::Text _optionButton;
		sf::Text _newGameButton;

		sf::VertexArray m_quadVertexArray;
		sf::Shader m_quadShader;

		sf::Shader m_coreshader;

		///////////////////////////////////
		short int music_count = 0;
		///////////////////////////////////

	private:

		ButtonStatus optionButtonStatus;
		ButtonStatus quitButtonStatus;
		ButtonStatus playButtonStatus;
		ButtonStatus newGameButtonStatus;
	};
}

