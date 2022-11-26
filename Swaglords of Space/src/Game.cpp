#include "Game.h"
#include "States/SplashState.h"
#include "States/LevelState.h"

namespace Swag
{
	Game::Game(int height, int width, std::string title)
	{
		_data->window.create(sf::VideoMode(height, width), title, sf::Style::Close | sf::Style::Titlebar);
		_data->machine.AddState(StateRef(new SplashState(this->_data)), true);
		//_data->machine.AddState(StateRef(new LevelState(this->_data)), true);

		_data->window.setKeyRepeatEnabled(false);
		SWAG_INFO("Game Created");
		this->Run();
	}

	void Game::Run()
	{
		float newTime = 0.f, frameTime = 0.f, interpolation = 0.f;

		float currentTime = this->_clock.getElapsedTime().asSeconds();

		float accumulator = 0.0f;

		while (this->_data->window.isOpen())
		{
			this->_data->machine.ProcessStateChanges();

			newTime = this->_clock.getElapsedTime().asSeconds();

			frameTime = newTime - currentTime;

			if (frameTime > 0.25f)
			{
				frameTime = 0.25f;
			}

			currentTime = newTime;
			accumulator += frameTime;

			while (accumulator >= dt)
			{
				this->_data->machine.GetActiveState()->HandleInput();

				this->_data->machine.GetActiveState()->Update(dt);

				accumulator -= dt;
			}

			interpolation = accumulator / dt;
			this->_data->machine.GetActiveState()->Draw(interpolation);
		}
	}
}