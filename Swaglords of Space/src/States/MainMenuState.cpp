#include "MainMenuState.h"
#include <sstream>

#include "Core/DEFFINITIONS.h"

#include <iostream>

#include "GameState.h"
#include "OptionState.h"
#include "LevelState.h"

namespace Swag
{
	MainMenuState::MainMenuState(GameDataRef data)
		: _data(data)
	{

	}

	void MainMenuState::Init()
	{
		this->optionButtonStatus = ButtonStatus::idle;
		this->quitButtonStatus = ButtonStatus::idle;
		this->playButtonStatus = ButtonStatus::idle;

		asserts.LoadMusic("BackgroundMusic", "Resources/sound/BackgroundMusic.ogg");
		asserts.GetMusic("BackgroundMusic").setLoop(true);

		asserts.LoadTexture("Background", MAIN_MENU_BACKGROUND_FILEPATH);
		asserts.LoadTexture("Play Button", PLAY_BUTTON);
		asserts.LoadTexture("Quit Button", QUIT_BUTTON);
		asserts.LoadTexture("Option Button", OPTION_BUTTON);
		asserts.LoadFont("Alger Font", ALGER_FONT);
		

		this->_background.setTexture(this->_data->assets.GetTexture("Background"));
		this->_playButton.setTexture(this->_data->assets.GetTexture("Play Button"));
		this->_quitButton.setFont(this->_data->assets.GetFont("Alger Font"));
		this->_optionButton.setFont(this->_data->assets.GetFont("Alger Font"));

		this->_optionButton.setString("Options");
		this->_optionButton.setCharacterSize(50);
		this->_quitButton.setString("QUIT");
		this->_quitButton.setCharacterSize(50);
		

		this->_playButton.setPosition((static_cast<float>(SCREEN_HEIGHT) / 2 - 70) - (this->_playButton.getGlobalBounds().width / 2), (static_cast<float>(SCREEN_WIDTH) / 2) - this->_playButton.getGlobalBounds().height / 2);
		this->_quitButton.setPosition(this->_data->window.getSize().x - this->_quitButton.getGlobalBounds().height - 90, this->_data->window.getSize().y - this->_quitButton.getGlobalBounds().width);
		this->_optionButton.setPosition((static_cast<float>(SCREEN_HEIGHT) / 2 + 100) - (this->_optionButton.getGlobalBounds().width / 2), (static_cast<float>(SCREEN_WIDTH) / 2 - 10) - this->_optionButton.getGlobalBounds().height / 2);

	}

	void MainMenuState::HandleInput()
	{
		sf::Event event{};

		while (this->_data->window.pollEvent(event))
		{
			if (sf::Event::Closed == event.type)
			{
				asserts.GetMusic("BackgroundMusic").stop();
				this->_data->window.close();
			}

			if (this->_data->input.isSpriteClicked(this->_playButton, sf::Mouse::Left, this->_data->window))
			{
				asserts.GetMusic("BackgroundMusic").stop();
				this->_data->machine.AddState(StateRef(new GameState(_data)), true);
			}

			if (this->_data->input.isTextClicked(this->_optionButton, sf::Mouse::Left, this->_data->window))
				this->_data->machine.AddState(StateRef(new OptionState(this->_data)), false);

			if (this->_data->input.isTextClicked(this->_quitButton, sf::Mouse::Left, this->_data->window))
				this->_data->window.close();
		}

	}

	void MainMenuState::Update(float dt)
	{
		if (this->_optionButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(this->_data->input.GetMousePosition(this->_data->window))))
			this->optionButtonStatus = ButtonStatus::hover;
		else
			this->optionButtonStatus = ButtonStatus::idle;

		if (this->_quitButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(this->_data->input.GetMousePosition(this->_data->window))))
			this->quitButtonStatus = ButtonStatus::hover;
		else
			this->quitButtonStatus = ButtonStatus::idle;

		if (this->_playButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(this->_data->input.GetMousePosition(this->_data->window))))
			this->playButtonStatus = ButtonStatus::hover;
		else
			this->playButtonStatus = ButtonStatus::idle;


		if (this->optionButtonStatus == ButtonStatus::hover)
			this->_optionButton.setFillColor(sf::Color::Green);

		if (this->quitButtonStatus == ButtonStatus::hover)
			this->_quitButton.setFillColor(sf::Color::Green);
		
		if (this->playButtonStatus == ButtonStatus::hover)
			this->_playButton.setColor(sf::Color::Green);


		if (this->optionButtonStatus == ButtonStatus::idle)
			this->_optionButton.setFillColor(sf::Color::White);

		if (this->quitButtonStatus == ButtonStatus::idle)
			this->_quitButton.setFillColor(sf::Color::White);

		if (this->playButtonStatus == ButtonStatus::idle)
			this->_playButton.setColor(sf::Color::White);

		if (this->music_count == 0)
		{
			asserts.GetMusic("BackgroundMusic").play();
			this->music_count++;
		}

	}

	void MainMenuState::Draw(float dt)
	{
		this->_data->window.clear();

		this->_data->window.draw(this->_background);
		this->_data->window.draw(this->_playButton);
		this->_data->window.draw(this->_quitButton);
		this->_data->window.draw(this->_optionButton);

		this->_data->window.display();
	}
}