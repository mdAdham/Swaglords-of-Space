#include "LevelState.h"

namespace Swag
{
	LevelState::LevelState(GameDataRef data)
		:_data(data)
	{

	}

	void LevelState::Init()
	{

	}

	void LevelState::HandleInput()
	{
		sf::Event event{};

		while (this->_data->window.pollEvent(event))
		{
			if (sf::Event::Closed == event.type)
				this->_data->window.close();
		}
	}

	void LevelState::Update(float dt)
	{

	}

	void LevelState::Draw(float dt)
	{
		this->_data->window.clear(sf::Color::Red);



		this->_data->window.display();
	}

	void LevelState::UpdateLevels()
	{
		this->_levels.WriteFile("Resources/levels/level" + std::to_string(this->_level++) + ".txt", "", 1);
		
	}
}