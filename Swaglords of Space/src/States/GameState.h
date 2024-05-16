#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "State/State.h"
#include "Game.h"
#include "Swaglords of Space/Player.h"
#include "Swaglords of Space/Bullet.h"
#include "Swaglords of Space/Enemy.h"

#include "Utils/Light.h"

namespace Swag
{
	class GameState : public State
	{
	public:

		GameState(GameDataRef data);
		~GameState();

		void Init();

		void HandleInput();
		void Update(float dt);
		void Draw(float dt);

	private:
		GameDataRef _data;

		sf::Sprite _background;

		sf::Sprite _homeButton;

	private:
		//Init
		void initVariables();
		void initTextures();
		void initGui();
		void initSystems();
		void initSound();
		void initSpeed();

		void initPlayer();
		void initEmemies();

	private:

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
		short int playerSpeed;
		bool screencap = false;
		short int capturetime = 0;
		short int Background_music_count = 0;
		sf::Clock clock;

		//Player
		Player* player;
		Light PlayerLight;
		sf::Vector2f lightPosition;
		
		//PlayerGui
		sf::RectangleShape playerHpBar;
		sf::RectangleShape playerHpBarBack;

		sf::RectangleShape playerboostBar;
		sf::RectangleShape playerboostBarBack;

		sf::VertexArray playerSpeedTEX1;
		sf::VertexArray playerSpeedTEX2;
		sf::VertexArray playerSpeedTEX3;

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
		void updateSpeed();
		void updateLight(float dt);

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

