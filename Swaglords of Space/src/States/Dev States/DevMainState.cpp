#include "DevMainState.h"
#include "Core/DEFFINITIONS.h"
#include <Windows.h>

namespace Swag
{
	DevMainState::DevMainState(GameDataRef data)
		:_data(data)
	{
		
	}

	void DevMainState::InitButtons()
	{
		this->_newGameButtonStatus = ButtonStatus::idle;
	}

	void DevMainState::InitText()
	{
		this->_data->assets.LoadFont("Party_font", PARTY_FONT);

		this->_CreateNewGame.setFont(this->_data->assets.GetFont("Party_font"));
		this->_CreateNewGame.setFillColor(sf::Color::Red);
		this->_CreateNewGame.setPosition(sf::Vector2f((DEV_SCREEN / 2) - 50, (DEV_SCREEN / 2) - 190));
		this->_CreateNewGame.setString("New Game");
		this->_CreateNewGame.setCharacterSize(40);
	}

	void DevMainState::updateButtonStates(float dt)
	{
		//Hover
		if (this->_CreateNewGame.getGlobalBounds().contains(static_cast<sf::Vector2f>(this->_data->input.GetMousePosition(this->_data->window))))
			this->_newGameButtonStatus = ButtonStatus::hover;
		else
			this->_newGameButtonStatus = ButtonStatus::idle;

		//Change of Color
		if (this->_newGameButtonStatus == ButtonStatus::hover)
			this->_CreateNewGame.setFillColor(sf::Color::Green);


		if (this->_newGameButtonStatus == ButtonStatus::idle)
			this->_CreateNewGame.setFillColor(sf::Color::White);
	}

	void DevMainState::Init()
	{
		this->InitButtons();
		this->InitText();
	}

	void DevMainState::HandleInput()
	{
		sf::Event ev{};

		while (this->_data->window.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
			{
				int msg = MessageBox(this->_data->window.getSystemHandle(), L"Do Want To Close???", L"Developer", 1);
				switch (msg)
				{
				case 1:
					this->_data->window.close();
					break;

				case 2:
					break;
				}
			}
		}
	}

	void DevMainState::Update(float dt)
	{
		this->updateButtonStates(dt);
	}

	void DevMainState::Draw(float dt)
	{
		this->_data->window.clear();

		this->_data->window.draw(this->_CreateNewGame);

		this->_data->window.display();
	}
}