#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "State/State.h"
#include "Game.h"
#include "Swaglords of Space/Player.h"
#include "Swaglords of Space/Bullet.h"
#include "Swaglords of Space/Enemy.h"

namespace Swag
{
	class GameState : public State
	{
	public:

		GameState(GameDataRef data);

		void Init();

		void HandleInput();
		void Update(float dt);
		void Draw(float dt);

	private:
		GameDataRef _data;

		sf::Sprite _background;
		sf::Image _backgroundImage;

		sf::Sprite _homeButton;

	private:
		//Init
		void initVariables();
		void initTextures();
		void initGui();
		void initSystems();
		void initSound();

		void initPlayer();
		void initEmemies();

	private:
		//Variables

		//Resources
		std::map<std::string, sf::Texture*> textures;
		std::vector<Bullet*> bullets;

		//GUI
		sf::Font font;
		sf::Text pointText;
		sf::Text PauseText;

		sf::Text gameOverText;

		//World
		sf::Sprite worldBackground;
		sf::Image screenshot;
		sf::Vector2u windowSize = this->_data->window.getSize();
		sf::Texture current_screen_texture;

		//Systems
		unsigned points;
		bool entityDeleted;
		short int level;
		bool screencap = false;
		short int capturetime = 0;
		short int Background_music_count = 0;

		//Player
		Player* player;

		//PlayerGui
		sf::RectangleShape playerHpBar;
		sf::RectangleShape playerHpBarBack;


		//Enemies
		float spawnerTimer;
		float spawnTimerMax;
		std::vector<Enemy*> ememies;

	public:
		//Functions
		void updatePollEvents();
		void updateInput();
		void updateGui();
		void updateWorld();
		void updateCollision();
		void updateBullets();
		void updateEnemies();
		void updateCombat();
		void updateLevel();

		void renderGui();
		void renderWorld();
		
		void updateStatus();
	private:
		enum class ButtonStatus
		{
			idle,
			hover,
			click
		};

		ButtonStatus _homeButtonStatus = ButtonStatus::idle;
	};
}

