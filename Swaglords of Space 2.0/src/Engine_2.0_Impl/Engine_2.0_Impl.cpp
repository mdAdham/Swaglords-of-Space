#include "Engine_2.0_Impl.h"
#include "Core/Core.hpp"

#include "Core/Deffinitions.hpp"

#include "Gui_Impl/Gui.hpp"

#include "States/MainMenuState.hpp"

namespace _Swag::Engine_2 {

	Engine_v2_Impl::Engine_v2_Impl()
	{
		_SWAG_TRACE("Engine v2.0 Created!");
	}

	Engine_v2_Impl::~Engine_v2_Impl()
	{
		this->_data->assets.ShutDown();
		_SWAG_TRACE("Engine v2.0 Destroyed!");
	}

	void Engine_v2_Impl::Initialize()
	{
		_SWAG_TRACE("Engine v2.0 Initialized!");

		sf::ContextSettings settings;
		settings.majorVersion = 4;
		settings.minorVersion = 2;
		
		this->_data->window.create(sf::VideoMode(WINDOW_WIDHT, WINDOW_HEIGHT, 32u), "Swag Space v2.0 - Engine v2.0", sf::Style::Close | sf::Style::Titlebar, settings);
		_SWAG_INFO("Window Created {0}x{1}", WINDOW_WIDHT, WINDOW_HEIGHT);
		//this->_data->window.create(sf::VideoMode::getDesktopMode(), "Swag Space v2.0 - Engine v2.0", sf::Style::Fullscreen, settings);
		//_SWAG_INFO("Window Created {0}x{1}", sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);

		this->_data->window.setFramerateLimit(120);

		_data->assets.LoadFont("Arial_Font", "arial.ttf");
		_defaulttext.setFont(_data->assets.GetFont("Arial_Font"));
		_defaulttext.setString("This is a default State");

		sf::VideoMode vm(_data->window.getSize().x, _data->window.getSize().y);

		_defaulttext.setCharacterSize(_gui::calcCharSize(vm, 52));
		_defaulttext.setOrigin(_defaulttext.getGlobalBounds().width / 2, _defaulttext.getGlobalBounds().height / 2);
		_defaulttext.setPosition(_gui::p2pX(50, vm), _gui::p2pY(50, vm));
		_defaulttext.setFillColor(sf::Color(100, 100, 100));

		this->_versionoftheeng.setFont(_data->assets.GetFont("Arial_Font"));
		this->_versionoftheeng.setCharacterSize(_gui::calcCharSize(vm, 145));
		this->_versionoftheeng.setString("BackupEngine v2.0");
		this->_versionoftheeng.setFillColor(sf::Color(200, 200, 200, 255));
		this->_versionoftheeng.setPosition(_gui::p2pX(0, vm), _gui::p2pY(0, vm));

		this->_data->machine.AddState(CreateRef<MainMenuState>(_data), false);
	}

	void Engine_v2_Impl::Run()
	{
		float newTime = 0.f, frameTime = 0.f, interpolation = 0.f;

		float currentTime = this->dtClock.getElapsedTime().asSeconds();

		float accumulator = 0.0f;

		while (this->_data->window.isOpen() && !this->_data->quit)
		{
			this->_data->machine.ProcessStateChanges();

			newTime = this->dtClock.getElapsedTime().asSeconds();

			frameTime = newTime - currentTime;

			if (frameTime > 0.25f)
			{
				frameTime = 0.25f;
			}

			currentTime = newTime;
			accumulator += frameTime;


			while (accumulator > dt)
			{
				while (this->_data->window.pollEvent(ev))
				{
					if (!this->_data->machine.IsEmpty())
					{
						if (ev.key.shift && ev.key.code == sf::Keyboard::Escape || ev.type == sf::Event::Closed)
							_data->window.close();
						this->_data->machine.GetActiveState()->OnEvent(ev);
					}
					else
						DefaultEvent();
				}
					

				if (!this->_data->machine.IsEmpty()) 
					this->_data->machine.GetActiveState()->OnUpdate(dt);

				accumulator -= dt;
			}

			interpolation = accumulator / dt;
			if (!this->_data->machine.IsEmpty())
			{
				_data->window.clear();
				_data->window.draw(_versionoftheeng);
				this->_data->machine.GetActiveState()->OnRender(interpolation);
			}
			else
				DefaultRender();
		}

		if (_data->quit)
			this->_data->window.close();

		this->_data->assets.ShutDown();
	}

	void Engine_v2_Impl::DefaultEvent() const
	{
		if (ev.key.shift && ev.key.code == sf::Keyboard::Escape || ev.type == sf::Event::Closed)
			_data->window.close();
	}

	void Engine_v2_Impl::DefaultRender()
	{
		_data->window.clear();

		sf::VideoMode vm(_data->window.getSize().x, _data->window.getSize().y);
		sf::View view(sf::FloatRect(0, 0, vm.width, vm.height));
		_data->window.setView(view);

		_defaulttext.setCharacterSize(_gui::calcCharSize(vm, 52));
		_defaulttext.setOrigin(_defaulttext.getGlobalBounds().width / 2, _defaulttext.getGlobalBounds().height / 2);
		_defaulttext.setPosition(static_cast<float>(WINDOW_WIDHT) / 2, static_cast<float>(WINDOW_HEIGHT) / 2);

		_defaulttext.setPosition(_gui::p2pX(50, vm), _gui::p2pY(50, vm));
		_data->window.draw(_defaulttext);

		_data->window.display();
	}

}