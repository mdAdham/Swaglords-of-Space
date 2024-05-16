#include <sstream>
#include "SplashState.h"

#include <iostream>

#include "Core/DEFFINITIONS.h"
#include "States/MainMenuState.h"

#include "Core/DEFFINITIONS.h"

namespace Swag
{
	SplashState::SplashState(GameDataRef data) : _data(data)
	{

	}

	void SplashState::Init()
	{
		this->_data->assets.LoadTexture("Game Background", SPLASH_SCENE_BACKGROUND_FILEPATH, true);

		_background.setTexture(this->_data->assets.GetTexture("Game Background"));

		asserts.LoadFont("Alger Font", ALGER_FONT);
		asserts.LoadFont("Party Font", PARTY_FONT);

		loadingText.setFont(asserts.GetFont("Alger Font"));
		loadingText.setCharacterSize(64);
		loadingText.setString("LOADING..");
		loadingText.setOrigin(sf::Vector2f(
		/*X*/	loadingText.getGlobalBounds().width / 2, 
		/*Y*/	loadingText.getGlobalBounds().height / 2));
		loadingText.setPosition(SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2);
	}

	void SplashState::HandleInput()
	{
		sf::Event event;

		while (this->_data->window.pollEvent(event))
		{
			if (sf::Event::Closed == event.type)
				this->_data->window.close();
		}
	}

	void SplashState::Update(float dt)
	{
		if (this->_clock.getElapsedTime().asSeconds() > SPLASH_STATE_SHOW_TIME)
		{
			//Switch to the Main Menu
			this->_data->machine.AddState(StateRef(new MainMenuState(this->_data)), true);
		}
	}

	void SplashState::Draw(float dt)
	{
		this->_data->window.clear(sf::Color::Red);

		this->_data->window.draw(this->_background);

		this->_data->window.draw(this->loadingText);

		this->_data->window.display();
	}
}