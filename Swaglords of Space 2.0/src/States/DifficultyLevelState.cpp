#include "DifficultyLevelState.hpp"
#include "Core/Log.hpp"
#include "Managers_Impl/InputManager.hpp"

#include "GameState.hpp"

namespace _Swag {
	DifficultyLevelState::DifficultyLevelState(Ref<GameData> data)
		: _data(data)
	{
	}

	DifficultyLevelState::~DifficultyLevelState()
	{
		
	}

	void DifficultyLevelState::Init()
	{
		_SWAG_INFO("Initializing Difficulty Level State!");
		vm = sf::VideoMode(_data->window.getSize().x, _data->window.getSize().y);

		_difficultyLevelSelector = CreateRef<_gui::DropDownList>(_gui::p2pX(30, vm), _gui::p2pY(20, vm),
			_gui::p2pX(13.f, vm), _gui::p2pY(6.f, vm),
			_data->assets.GetFont("Arial_Font"),
			_difflevellist, 4, 0);
		_SWAG_INFO("Entering to Difficulty Level State!");

		_diffText.setFont(_data->assets.GetFont("Arial_Font"));
		_diffText.setCharacterSize(_gui::calcCharSize(vm, 60));
		_diffText.setString("Difficulty: ");
		_diffText.setPosition(_gui::p2pX(17, vm), _gui::p2pY(20, vm));

		_Play_Btn = CreateRef<_gui::Button>(
			_gui::p2pX(40, vm), _gui::p2pY(60, vm),
			_gui::p2pX(10.f, vm), _gui::p2pY(6.f, vm),
			&_data->assets.GetFont("Arial_Font"), "PLAY", _gui::calcCharSize(vm),
			sf::Color(200, 200, 200, 255), sf::Color(220, 220, 220, 255), sf::Color(255, 255, 255, 255),
			sf::Color(50, 50, 50, 200), sf::Color(40, 40, 40, 200), sf::Color(30, 30, 30, 200),
			sf::Color(0, 0, 0, 0), sf::Color(60, 30, 30, 100), sf::Color(0, 0, 0, 0));
	}
	
	void DifficultyLevelState::OnEvent(sf::Event& ev)
	{
	}
	
	void DifficultyLevelState::OnUpdate(float dt)
	{
		_difficultyLevelSelector->update(InputManager::GetMousePosition(_data->window), dt);
		_Play_Btn->update(InputManager::GetMousePosition(_data->window));

		if (_Play_Btn->isPressed())
		{
			GameState::GameModes mode;

			_SWAG_DEBUGS("Difficulty level Index: {0}", _difficultyLevelSelector->getActiveElementId());

			switch (_difficultyLevelSelector->getActiveElementId())
			{
			case 0: { //American
				mode.enemie_spawner_Time_Max = 55.f;
				mode.bullet_speed = 3500.f;
				mode.enemy_damage_factor = 1;
				mode.enemie_speed_factor = .8f;
				mode.enemy_points_factor = 4;

				mode.player_speed = 6.0f;
				mode.player_attack_cooldown_max = 8.0f;
				mode.player_max_hp = 200;
				mode.player_max_boost = 600;
			}break;
			case 1: { //Intermediate
				mode.enemie_spawner_Time_Max = 50.f;
				mode.bullet_speed = 3000.f;
				mode.enemy_damage_factor = 1;
				mode.enemie_speed_factor = 1;
				mode.enemy_points_factor = 3;

				mode.player_speed = 7.10f;
				mode.player_attack_cooldown_max = 10.0f;
				mode.player_max_hp = 100;
				mode.player_max_boost = 300;
			}break;
			case 2: { //Pro
				mode.enemie_spawner_Time_Max = 35.f;
				mode.bullet_speed = 3500.f;
				mode.enemy_damage_factor = 2;
				mode.enemie_speed_factor = 1.5;
				mode.enemy_points_factor = 2;

				mode.player_speed = 10.0f;
				mode.player_attack_cooldown_max = 14.0f;
				mode.player_max_hp = 50;
				mode.player_max_boost = 150;
			}break;
			case 3: { //Indian
				mode.enemie_spawner_Time_Max = 20.f;
				mode.bullet_speed = 4000.f;
				mode.enemy_damage_factor = 4;
				mode.enemie_speed_factor = 2;
				mode.enemy_points_factor = 1;

				mode.player_speed = 13.1f;
				mode.player_attack_cooldown_max = 18.0f;
				mode.player_max_hp = 25;
				mode.player_max_boost = 75;
			}break;
			default:
				mode.enemie_spawner_Time_Max = 60.f;
				mode.bullet_speed = 3500.f;
				mode.enemy_damage_factor = .5;
				mode.enemie_speed_factor = .5;
				mode.enemy_points_factor = 4;

				mode.player_speed = 6.0f;
				mode.player_attack_cooldown_max = 8.0f;
				mode.player_max_hp = 200;
				mode.player_max_boost = 600;
				break;
			}
			_data->machine.AddState(CreateRef<GameState>(_data, mode), true); return;
		}
	}
	
	void DifficultyLevelState::OnRender(float dt)
	{
		_data->window.draw(this->_diffText);
		_difficultyLevelSelector->render(_data->window);
		_Play_Btn->render(_data->window);

		_data->window.display();
	}
	
}