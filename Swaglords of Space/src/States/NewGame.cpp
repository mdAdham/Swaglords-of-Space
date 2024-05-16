#include "NewGame.h"

namespace Swag {

	Swag::NewGameState::NewGameState(GameDataRef data)
		: _data(data)
	{
	}

	void Swag::NewGameState::Init()
	{
	}

	void Swag::NewGameState::HandleInput()
	{
		sf::Event event{};

		while (this->_data->window.pollEvent(event))
		{
			if (event.key.code == sf::Keyboard::Escape)
				this->_data->window.close();

			if (event.type == sf::Event::Closed)
				this->_data->window.close();
		}
	}

	void Swag::NewGameState::Update(float dt)
	{
	}

	void Swag::NewGameState::Draw(float dt)
	{
		this->_data->window.clear();

		this->_data->window.display();
	}
}