#include "MainMenuState.hpp"
#include "Managers_Impl/InputManager.hpp"
#include "Core/Deffinitions.hpp"

#include "DifficultyLevelState.hpp"

namespace _Swag {
	MainMenuState::MainMenuState(Ref<GameData> data)
		:_data(data)
	{
		_SWAG_INFO("Main Menu State has been created!");
	}

	void MainMenuState::Init()
	{
		_SWAG_TRACE("Main Menu State has been Initializeed!");

		//Buttons
		sf::VideoMode vm(_data->window.getSize().x, _data->window.getSize().y);

		this->_buttons["PLAY_STATE"] = CreateRef<_gui::Button>(
			_gui::p2pX(15.f, vm), _gui::p2pY(50.f, vm),
			_gui::p2pX(13.f, vm), _gui::p2pY(6.f, vm),
			&_data->assets.GetFont("Arial_Font"), "Play", _gui::calcCharSize(vm),
			sf::Color(200, 200, 200, 200), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50),
			sf::Color(70, 70, 70, 10), sf::Color(150, 150, 150, 10), sf::Color(20, 20, 20, 10)
		);

		this->_buttons["SETTINGS_STATE"] = CreateRef<_gui::Button>(
			_gui::p2pX(15.f, vm), _gui::p2pY(60.f, vm),
			_gui::p2pX(13.f, vm), _gui::p2pY(6.f, vm),
			&_data->assets.GetFont("Arial_Font"), "Settings", _gui::calcCharSize(vm),
			sf::Color(200, 200, 200, 200), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50),
			sf::Color(70, 70, 70, 10), sf::Color(150, 150, 150, 10), sf::Color(20, 20, 20, 10)
		);

		this->_buttons["QUIT_STATE"] = CreateRef<_gui::Button>(
			_gui::p2pX(15.f, vm), _gui::p2pY(70.f, vm),
			_gui::p2pX(13.f, vm), _gui::p2pY(6.f, vm),
			&_data->assets.GetFont("Arial_Font"), "Quit", _gui::calcCharSize(vm),
			sf::Color(200, 200, 200, 200), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50),
			sf::Color(70, 70, 70, 10), sf::Color(150, 150, 150, 10), sf::Color(20, 20, 20, 10)
		);

		this->_data->assets.LoadTexture("Ship_Texture", SHIP_TEXTURE, true);

		this->_ShipLogo.setTexture(_data->assets.GetTexture("Ship_Texture"));
		this->_ShipLogo.setOrigin(_ShipLogo.getGlobalBounds().width / 2, _ShipLogo.getGlobalBounds().height / 2);
		this->_ShipLogo.setPosition(_gui::p2pX(70, vm), _gui::p2pY(50, vm));
		this->_ShipLogo.setScale(3, 3);

		this->_GameName.setFont(_data->assets.GetFont("Arial_Font"));
		this->_GameName.setCharacterSize(_gui::calcCharSize(vm, 34));
		this->_GameName.setString("Swaglords of Space v2.0");
		this->_GameName.setPosition(_gui::p2pX(10, vm), _gui::p2pY(20, vm));

		this->_data->assets.LoadTexture("Radial_Button_UnChecked", Radial_Button_UC, true);
		this->_data->assets.LoadTexture("Radial_Button_Checked", Radial_Button_UC, true);
	}
	
	void MainMenuState::OnEvent(sf::Event& ev)
	{
		if (ev.type == sf::Event::Resized)
		{
			_SWAG_DEBUGS("Window Resized");

			sf::VideoMode vm(_data->window.getSize().x, _data->window.getSize().y);

			this->_buttons["PLAY_STATE"]->updatePosition(_gui::p2pX(15.f, vm), _gui::p2pY(50.f, vm),
				_gui::p2pX(13.f, vm), _gui::p2pY(6.f, vm), _gui::calcCharSize(vm));
			this->_buttons["SETTINGS_STATE"]->updatePosition(_gui::p2pX(15.f, vm), _gui::p2pY(60.f, vm),
				_gui::p2pX(13.f, vm), _gui::p2pY(6.f, vm), _gui::calcCharSize(vm));
			this->_buttons["QUIT_STATE"]->updatePosition(_gui::p2pX(15.f, vm), _gui::p2pY(70.f, vm),
				_gui::p2pX(13.f, vm), _gui::p2pY(6.f, vm), _gui::calcCharSize(vm));

			this->_ShipLogo.setPosition(_gui::p2pX(70, vm), _gui::p2pY(50, vm));
		}
		
		if (ev.key.code == sf::Keyboard::Add)
			_data->window.setSize(sf::Vector2u(_data->window.getSize().x + 10, _data->window.getSize().y + 10));
		if (ev.key.code == sf::Keyboard::Subtract)
			_data->window.setSize(sf::Vector2u(_data->window.getSize().x - 10, _data->window.getSize().y - 10));
		
	}
	
	void MainMenuState::OnUpdate(float dt)
	{
		for (auto& it : this->_buttons)
		{
			it.second->update(InputManager::GetMousePosition(_data->window));
		}

		if (this->_buttons["PLAY_STATE"]->isPressed())
		{
			_SWAG_INFO("Entering to Play State!");
			_data->machine.AddState(CreateRef<DifficultyLevelState>(_data), false);
		}

		if (this->_buttons["SETTINGS_STATE"]->isPressed())
		{
			_SWAG_INFO("Entering to Settings State!");
		}

		if (this->_buttons["QUIT_STATE"]->isPressed())
		{
			_SWAG_INFO("Quiting the Game!");
			this->_data->machine.RemoveState();
			this->_data->quit = true;
		}

	}
	
	void MainMenuState::OnRender(float dt)
	{
		sf::VideoMode vm(_data->window.getSize().x, _data->window.getSize().y);
		sf::View view(sf::FloatRect(0, 0, vm.width, vm.height));
		_data->window.setView(view);

		//_data->window.clear(sf::Color::Black);

		_data->window.draw(_GameName);
		_data->window.draw(_ShipLogo);


		for (auto& it : this->_buttons)
		{
			it.second->render(_data->window);
		}

		_data->window.display();
	}

	void MainMenuState::OnDestroy()
	{
		auto it = this->_buttons.begin();
		for (it = this->_buttons.begin(); it != this->_buttons.end(); ++it)
		{
			it->second.~shared_ptr();
		}
	}

}