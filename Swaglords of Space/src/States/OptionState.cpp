#include "OptionState.h"

namespace Swag
{
	OptionState::OptionState(GameDataRef data)
		: _data(data)
	{

	}

	void OptionState::Init()
	{
		this->levelBackTex.loadFromFile("");
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
		}
	}

	void OptionState::Update(float dt)
	{

	}

	void OptionState::Draw(float dt)
	{
		this->_data->window.clear(sf::Color::Blue);



		this->_data->window.display();
	}
}