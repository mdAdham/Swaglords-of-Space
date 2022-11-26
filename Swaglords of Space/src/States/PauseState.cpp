#include "PauseState.h"
#include "Core/DEFFINITIONS.h"
#include <iostream>

#include "MainMenuState.h"

namespace Swag
{
	PauseState::PauseState(GameDataRef data)
		: _data(data)
	{

	}

	void PauseState::Init()
	{
		this->pauseButtonStatus = ButtonStatus::idle;
		this->homeButtonStatus = ButtonStatus::idle;

		this->_data->assets.LoadTexture("Pause Button", PAUSE_BUTTON);
		this->_data->assets.LoadTexture("Home Button", HOME_BUTTON);

		this->_pauseButton.setTexture(this->_data->assets.GetTexture("Pause Button"));
		this->_background.setTexture(this->_data->assets.GetTexture("Background"));
		this->_homeButton.setTexture(this->_data->assets.GetTexture("Home Button"));

		this->_pauseButton.setPosition((static_cast<float>(SCREEN_HEIGHT) / 2.5) - (this->_pauseButton.getGlobalBounds().width / 2), (static_cast<float>(SCREEN_WIDTH) / 2) - this->_pauseButton.getGlobalBounds().height / 2);
		this->_homeButton.setPosition((static_cast<float>(SCREEN_HEIGHT) / 1.5) - (this->_homeButton.getGlobalBounds().width / 2), (static_cast<float>(SCREEN_WIDTH) / 2) - this->_homeButton.getGlobalBounds().height / 2);
	}

	void PauseState::HandleInput()
	{
		sf::Event event{};
		while (this->_data->window.pollEvent(event))
		{
			if (sf::Event::Closed == event.type)
				this->_data->window.close();
			
			if (this->_data->input.isSpriteClicked(this->_pauseButton, sf::Mouse::Left, this->_data->window))
			{
				this->_data->machine.RemoveState();
			}

			if (this->_data->input.isSpriteClicked(this->_homeButton, sf::Mouse::Left, this->_data->window))
			{
				this->_data->machine.RemoveState();
				this->_data->machine.AddState(StateRef(new MainMenuState(this->_data)), true);
			}
		}
	}

	void PauseState::Update(float dt)
	{
		if (this->_pauseButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(this->_data->input.GetMousePosition(this->_data->window))))
			this->pauseButtonStatus = ButtonStatus::hover;
		else
			this->pauseButtonStatus = ButtonStatus::idle;

		if (this->_homeButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(this->_data->input.GetMousePosition(this->_data->window))))
			this->homeButtonStatus = ButtonStatus::hover;
		else
			this->homeButtonStatus = ButtonStatus::idle;

		if (this->pauseButtonStatus == ButtonStatus::hover)
			this->_pauseButton.setColor(sf::Color::Green);

		if (this->homeButtonStatus == ButtonStatus::hover)
			this->_homeButton.setColor(sf::Color::Green);

		if (this->pauseButtonStatus == ButtonStatus::idle)
			this->_pauseButton.setColor(sf::Color::White);

		if (this->homeButtonStatus == ButtonStatus::idle)
			this->_homeButton.setColor(sf::Color::White);
	}

	void PauseState::Draw(float dt)
	{
		this->_data->window.clear();

		this->_data->window.draw(this->_background);
		this->_data->window.draw(this->_pauseButton);
		this->_data->window.draw(this->_homeButton);

		this->_data->window.display();
	}
}