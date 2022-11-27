#include "Game.h"
#include "States/SplashState.h"
#include "States/LevelState.h"

#include "States/Dev States/DevMainState.h"

namespace Swag
{
	Game::Game(int height, int width, std::string title, int DevWin_height, int DevWin_width, std::string DevWin_Title)
	{
		_data->window.create(sf::VideoMode(height, width), title, sf::Style::Close | sf::Style::Titlebar);
		_data->machine.AddState(StateRef(new SplashState(this->_data)), true);
		
		_data2->window.create(sf::VideoMode(DevWin_width, DevWin_height), DevWin_Title, sf::Style::Close | sf::Style::Titlebar);
		_data2->machine.AddState(StateRef(new DevMainState(this->_data2)), true);

		_data->window.setKeyRepeatEnabled(false);
		SWAG_INFO("Game Created");
		this->Run();
	}

	void Game::Run()
	{
		float newTime = 0.f, frameTime = 0.f, interpolation = 0.f;

		float currentTime = this->_clock.getElapsedTime().asSeconds();

		float accumulator = 0.0f;

		while (this->_data->window.isOpen() || this->_data2->window.isOpen())
		{
			this->_data->machine.ProcessStateChanges();
			this->_data2->machine.ProcessStateChanges();

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
				this->_data2->machine.GetActiveState()->HandleInput();

				this->_data->machine.GetActiveState()->Update(dt);
				this->_data2->machine.GetActiveState()->Update(dt);

				accumulator -= dt;
			}

			interpolation = accumulator / dt;
			this->_data->machine.GetActiveState()->Draw(interpolation);
			this->_data2->machine.GetActiveState()->Draw(interpolation);
		}
	}
}