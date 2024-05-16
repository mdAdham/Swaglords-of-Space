#pragma once

#include "State_Impl/State.hpp"
#include "Core/Core.hpp"
#include "Core/AppData.hpp"
#include "Gui_Impl/Gui.hpp"

#include "Game_Impl/Player.hpp"
#include "Game_Impl/Bullet.hpp"
#include "Game_Impl/Enemy.hpp"

namespace _Swag {
	class GameState : public State
	{
	public:
		struct GameModes
		{
			float enemie_spawner_Time_Max = 0;
			float bullet_speed = 0;
			float enemie_speed_factor = 0;
			int enemy_damage_factor = 0;
			int enemy_points_factor = 0;
	
			float player_speed = 0;
			float player_attack_cooldown_max = 0;
			int player_max_hp = 0;
			int player_max_boost = 0;
		};
		
		GameState(Ref<GameData> data, GameModes modes);

		// Inherited via State
		void Init() override;

		void OnEvent(sf::Event& ev) override;

		void OnUpdate(float dt) override;

		void OnRender(float dt) override;

	private:
		Ref<GameData> _data;
		GameModes _modes;


	private:
		//PLAYER
		Ref<Player> _Player;
		Ref<_gui::ProgressBar> _playerHpBar;
		Ref<_gui::ProgressBar> _playerBoostBar;
		unsigned points = 0;

		std::vector<Ref<Bullet>> _bullets;

		//ENEMY
		float spawnerTimer = 0;
		float spawnerTimerMax = 0;
		std::vector<Ref<Enemy>> _enemies;
		bool allenemiedeleted = false;

	private:
		void UpdateGui();
	};
}