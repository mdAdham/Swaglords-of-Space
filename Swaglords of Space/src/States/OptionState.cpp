#include "OptionState.h"

namespace Swag
{
	OptionState::OptionState(GameDataRef data)
		: _data(data)
	{
		
	}


	void OptionState::initText()
	{
		this->MusicONOFF.setFont(this->_data->assets.GetFont("Alger Font"));
		
		//this->MusicONOFF.setOrigin(this->MusicONOFF.getGlobalBounds().height / 2, this->MusicONOFF.getGlobalBounds().width / 2);

		this->MusicONOFF.setCharacterSize(60);
		this->MusicONOFF.setPosition(sf::Vector2f(_data->window.getSize().x / 3, _data->window.getSize().y / 3));
		this->MusicONOFF.setString("Music: Off");
	}

	void OptionState::Init()
	{
		this->initText();
	}

	void OptionState::HandleInput()
	{
		sf::Event event{};

		while (this->_data->window.pollEvent(event))
		{
			if (sf::Event::Closed == event.type)
				this->_data->window.close();

			if (sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				this->_data->machine.RemoveState();
			if (_data->input.isTextClicked(this->MusicONOFF, sf::Mouse::Left, this->_data->window))
			{
				if (event.KeyReleased && event.key.code == sf::Mouse::Left)
				{
					if (this->isPressed == false)
					{
						this->MusicONOFF.setString("Music: Off");
						this->isPressed = true;
						this->_data->assets.GetMusic("BackgroundMusic").setVolume(0);
					}
					else
					{
						this->MusicONOFF.setString("Music: ON");
						this->isPressed = false;
						this->_data->assets.GetMusic("BackgroundMusic").setVolume(100);
					}
				}
			}
		}
	}

	void OptionState::Update(float dt)
	{
		if (this->MusicONOFF.getString() == "Music: ON")
		{
			//SWAG_TRACE("Music On");
		}
		if (this->MusicONOFF.getString() == "Music: Off")
		{
			//SWAG_TRACE("Music Off");
		}

		if (this->_data->assets.GetMusic("BackgroundMusic").getVolume() == 0)
		{
			this->MusicONOFF.setString("Music: Off");
		}
		if (this->_data->assets.GetMusic("BackgroundMusic").getVolume() > 0)
		{
			this->MusicONOFF.setString("Music: ON");
		}
	}

	void OptionState::Draw(float dt)
	{
		this->_data->window.clear(sf::Color::Blue);

		this->_data->window.draw(this->MusicONOFF);

		this->_data->window.display();
	}
}