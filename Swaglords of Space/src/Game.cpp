#include "Core/DEFFINITIONS.h"
#include "Game.h"
#include "States/Dev States/DevMainState.h"
#include "States/LevelState.h"
#include "States/SplashState.h"

namespace Swag
{
	Game::Game(int width, int height, std::string title, int DevWin_height, int DevWin_width, std::string DevWin_Title ,bool DevWin)
		:EnableDevWindow(DevWin)
	{

		sf::ContextSettings settings;
		_data->window.create(sf::VideoMode::getDesktopMode(), title, sf::Style::Titlebar | sf::Style::Close, settings);
		_data->machine.AddState(StateRef(new SplashState(this->_data)), true);
		
		_data->window_t.create(_data->window.getSize().x, _data->window.getSize().y);

		if (DevWin)
		{
		_data2->window.create(sf::VideoMode(DevWin_width, DevWin_height), DevWin_Title, sf::Style::Close | sf::Style::Titlebar);
		_data2->machine.AddState(StateRef(new DevMainState(this->_data2)), true);

		}

		_data->window.setKeyRepeatEnabled(true);
		//_data->window.setVerticalSyncEnabled(true);
		
		this->AppIcon = Asset::DatToImg(APP_ICON_FILEPATH);
		this->_data->window.setIcon(this->AppIcon.getSize().x, this->AppIcon.getSize().y, this->AppIcon.getPixelsPtr());
		//this->_data->window.setFramerateLimit(144);

		
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
			if(EnableDevWindow)
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
				if(EnableDevWindow)
					this->_data2->machine.GetActiveState()->HandleInput();

				this->_data->machine.GetActiveState()->Update(dt);
				if(EnableDevWindow)
					this->_data2->machine.GetActiveState()->Update(dt);

				accumulator -= dt;
			}

			this->_data->machine.GetActiveState()->DontDestroyOnLoad();

			interpolation = accumulator / dt;
			this->_data->machine.GetActiveState()->Draw(interpolation);

			if(EnableDevWindow)
				this->_data2->machine.GetActiveState()->Draw(interpolation);
		}

		asserts.ShutDown();
	}
}