#include "GameState.h"
#include <sstream>
#include "Core/DEFFINITIONS.h"

#include <iostream>

#include "PauseState.h"
#include "MainMenuState.h"

namespace Swag
{
	void GameState::initVariables()
	{
		this->level = 1;
	}

	void GameState::initTextures()
	{
		this->textures["BULLET"] = new sf::Texture();
		if (!this->textures["BULLET"]->loadFromFile("Resources/res/Bullet.png"))
			SWAG_ERROR("Failed to load Bullet Image");

	}

	void GameState::initGui()
	{
		//Load Fonts
		if (!this->font.loadFromFile("Resources/fonts/PartyConfettiRegular.ttf"))
			SWAG_ERROR("Failed to load Font");

		//Init point text
		this->pointText.setPosition(1600.f, 25.f);
		this->pointText.setFont(this->font);
		this->pointText.setCharacterSize(40);
		this->pointText.setFillColor(sf::Color::White);
		this->pointText.setString("TEST");

		this->PauseText.setPosition(1600.f, 1000.f);
		this->PauseText.setFont(this->font);
		this->PauseText.setCharacterSize(20);
		this->PauseText.setFillColor(sf::Color::White);
		this->PauseText.setString("Press P for Pause the Game");

		this->gameOverText.setFont(this->font);
		this->gameOverText.setCharacterSize(60);
		this->gameOverText.setFillColor(sf::Color::Red);
		this->gameOverText.setString("Game Over!");
		this->gameOverText.setPosition(
			this->_data->window.getSize().x / 2.f - this->gameOverText.getGlobalBounds().width / 2.f,
			this->_data->window.getSize().y / 2.f - this->gameOverText.getGlobalBounds().height / 2.f);

		//Init Player Gui
		this->playerHpBar.setSize(sf::Vector2f(300.f, 25.f));
		this->playerHpBar.setFillColor(sf::Color::Red);
		this->playerHpBar.setPosition(sf::Vector2f(20.f, 20.f));

		this->playerHpBarBack = this->playerHpBar;
		this->playerHpBarBack.setFillColor(sf::Color(25, 25, 25, 200));
	}

	void GameState::initSystems()
	{
		this->points = 0;
		this->entityDeleted = false;
	}

	void GameState::initSound()
	{
		if (!this->soundBuffers["FIRE_BUFFER"].loadFromFile("Resources/sound/Bullet Sound.wav"))
		{
			SWAG_ERROR("ERROR::GAME::INITSOUND::Failed to Load Bullet Sound");
		}else
		{ this->sounds["FIRE_SOUND"].setBuffer(this->soundBuffers["FIRE_BUFFER"]); }


		if (!this->soundBuffers["BREAK_BUFFER"].loadFromFile("Resources/sound/Rock Break.wav"))
		{
			SWAG_ERROR("ERROR::GAME::INITSOUND::Failed to Load Rock Break Sound");
		}else
		{ this->sounds["BREAK_SOUND"].setBuffer(this->soundBuffers["BREAK_BUFFER"]); }

	}

	void GameState::initPlayer()
	{
		this->player = new Player();
	}

	void GameState::initEmemies()
	{
		this->spawnTimerMax = 50.f;
		this->spawnerTimer = this->spawnTimerMax;
	}

	GameState::GameState(GameDataRef data)
		: _data(data)
	{
		
	}

	void GameState::Init()
	{
		this->_data->assets.LoadTexture("Game Background", GAME_BACKGROUND_FILEPATH);
		this->_data->assets.LoadTexture("Home Button", HOME_BUTTON);
		
		this->soundBuffers["BackgroungMusicBuffer"].loadFromFile("Resources/sound/BackgroundMusic.ogg");
		this->sounds["BackgroundMusic"].setBuffer(this->soundBuffers["BackgroungMusicBuffer"]);
		this->sounds["BackgroundMusic"].setLoop(true);


		_background.setTexture(this->_data->assets.GetTexture("Game Background"));
		_homeButton.setTexture(this->_data->assets.GetTexture("Home Button"));
		_backgroundImage.loadFromFile(GAME_BACKGROUND_FILEPATH);

		_homeButton.setPosition(static_cast<float>((SCREEN_HEIGHT) / 2.1), static_cast<float>((SCREEN_WIDTH) / 1.8));

		this->initVariables();
		this->initTextures();
		this->initGui();
		this->initSystems();
		this->initPlayer();
		this->initEmemies();
		this->initSound();
	}

	void GameState::HandleInput()
	{
		sf::Event event{};
		

		while (this->_data->window.pollEvent(event))
		{
			if (sf::Event::Closed == event.type)
			{
				this->sounds["BackgroundMusic"].stop();
				this->_data->window.close();
			}

			if (sf::Event::LostFocus == event.type)
				this->_data->window.clear();

			if (event.key.code == sf::Keyboard::C)
			{
				this->current_screen_texture.create(this->windowSize.x, this->windowSize.y);
				this->screencap = true;

			}

			if (this->player->getHp() == 0)
			{
				if (this->_data->input.isSpriteClicked(this->_homeButton, sf::Mouse::Left, this->_data->window))
				{
					SWAG_TRACE("Player Points - {0}", this->points);
					this->sounds["BackgroundMusic"].stop();
					this->_data->machine.AddState(StateRef(new MainMenuState(this->_data)), true);
				}
			}

			if (this->player->getHp() != 0)
			{
				if (event.Event::KeyPressed && event.Event::key.code == sf::Keyboard::P)
				{
					
					this->_data->machine.AddState(StateRef(new PauseState(this->_data)), false);
				}
			}
		}

	}

	void GameState::Update(float dt)
	{
		if (this->player->getHp() != 0)
		{
			this->updateInput();

			this->player->update();

			this->updateCollision();

			this->updateBullets();

			this->updateEnemies();

			this->updateCombat();

			this->updateGui();

			this->updateWorld();

			this->updateLevel();

		}
		else
		{
			if (this->entityDeleted == false)
			{
				for (auto* i : this->bullets)
				{
					delete i;
				}

				//Delete Enemies
				for (auto* i : this->ememies)
				{
					delete i;
				}
				this->entityDeleted = true;
			SWAG_TRACE("Player Points - {0}",this->points);
			SWAG_INFO("Player Died");
			}
			this->updateStatus();
		}
		if (this->screencap)
		{
			this->current_screen_texture.update(this->_data->window);
			this->screenshot = this->current_screen_texture.copyToImage();
			if (!this->screenshot.saveToFile("Resources/Capture/Capture" + std::to_string(this->capturetime++) + ".jpg"))
				SWAG_WARN("Cannot Save to File");
			this->screencap = false;
		}
		if (this->Background_music_count == 0)
		{
			this->sounds["BackgroundMusic"].play();
			this->Background_music_count++;
		}
	}

	void GameState::Draw(float dt)
	{
		this->_data->window.clear();

		this->_data->window.draw(this->_background);
		this->player->render(this->_data->window);

		if (this->player->getHp() != 0)
		{
			for (auto* bullet : this->bullets)
			{
				bullet->render(&this->_data->window);
			}

			for (auto* enemy : this->ememies)
			{
				enemy->render(&this->_data->window);
			}

		}
		
		this->renderGui();

		//Game Over screan
		if (this->player->getHp() <= 0)
		{
			this->_data->window.draw(this->gameOverText);
			this->_data->window.draw(this->_homeButton);
		}

		this->_data->window.display();
	}

	//Functions
	void GameState::updatePollEvents()
	{
		sf::Event ev{};
		while (this->_data->window.pollEvent(ev))
		{
			if (ev.Event::type == sf::Event::Closed)
				this->_data->window.close();
			if (ev.Event::KeyPressed && ev.Event::key.code == sf::Keyboard::Escape)
				this->_data->window.close();
			
		}
	}

	void GameState::updateInput()
	{
		//Move player
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			this->player->move(-1.f, 0.f);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			this->player->move(1.f, 0.f);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			this->player->move(0.f, -1.f);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			this->player->move(0.f, 1.f);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->player->canAttack() || sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && this->player->canAttack())
		{
			this->sounds["FIRE_SOUND"].play();
			this->bullets.push_back(new Bullet(this->textures["BULLET"], this->player->getPos().x + this->player->getBounds().width / 2.f - 5.f,
				this->player->getPos().y,
				0.f,
				-1.f,
				5.f
			)
			);
		}
	}

	void GameState::updateGui()
	{
		std::stringstream ss;

		ss << "Points: " << this->points;

		this->pointText.setString(ss.str());

		//Update player Gui
		float hpPercent = static_cast<float>(this->player->getHp()) / this->player->getHpMax();
		this->playerHpBar.setSize(sf::Vector2f(300.f * hpPercent, this->playerHpBar.getSize().y));
	}

	void GameState::updateWorld()
	{

	}

	void GameState::updateCollision()
	{
		//Left World Collision
		if (this->player->getBounds().left < 0.f)
		{
			this->player->setPosition(0.f, this->player->getBounds().top);
		}

		//Right world Collision
		else if (this->player->getBounds().left + this->player->getBounds().width >= this->_data->window.getSize().x)
		{
			this->player->setPosition(this->_data->window.getSize().x - this->player->getBounds().width, this->player->getBounds().top);
		}

		//Top world Collision
		if (this->player->getBounds().top < 0.f)
		{
			this->player->setPosition(this->player->getBounds().left, 0.f);
		}

		//Bottom world Collision
		else if (this->player->getBounds().top + this->player->getBounds().height >= this->_data->window.getSize().y)
		{
			this->player->setPosition(this->player->getBounds().left, this->_data->window.getSize().y - this->player->getBounds().height);
		}
	}

	void GameState::updateBullets()
	{
		unsigned counter = 0;
		for (auto* bullet : this->bullets)
		{
			bullet->update();

			//Bullet Culling (top of screan)
			if (bullet->getBounds().top + bullet->getBounds().height <= 0.f)
			{
				//Deleted Bullet
				delete this->bullets.at(counter);
				this->bullets.erase(this->bullets.begin() + counter);


			}
			++counter;
		}
	}

	void GameState::updateEnemies()
	{
		//Spawning
		this->spawnerTimer += 0.5;
		if (this->spawnerTimer >= this->spawnTimerMax)
		{
			this->ememies.push_back(new Enemy(rand() % this->_data->window.getSize().x + 10.f, -100));
			this->spawnerTimer = 0.f;
		}

		//Update
		unsigned counter = 0;
		for (auto* enemy : this->ememies)
		{
			enemy->update();

			//Bullet Culling (top of screan)
			if (enemy->getBounds().top > this->_data->window.getSize().y)
			{
				//Deleted enemy
				delete this->ememies.at(counter);
				this->ememies.erase(this->ememies.begin() + counter);
			}
			//Enemy player collision
			else if (enemy->getBounds().intersects(this->player->getBounds()))
			{
				this->player->loseHp(this->ememies.at(counter)->getDamage());
				delete this->ememies.at(counter);
				this->ememies.erase(this->ememies.begin() + counter);

				this->sounds["BREAK_SOUND"].play();
			}

			++counter;
		}
	}

	void GameState::updateCombat()
	{
		for (int i = 0; i < this->ememies.size(); ++i)
		{
			bool enemy_deleted = false;
			for (size_t k = 0; k < this->bullets.size() && enemy_deleted == false; k++)
			{
				if (this->ememies[i]->getBounds().intersects(this->bullets[k]->getBounds()))
				{
					this->points += this->ememies[i]->getPoints();

					delete this->ememies[i];
					this->ememies.erase(this->ememies.begin() + i);

					delete this->bullets[k];
					this->bullets.erase(this->bullets.begin() + k);

					this->sounds["BREAK_SOUND"].play();
					enemy_deleted = true;
				}
			}
		}
	}

	void GameState::updateLevel()
	{
		if (this->points > 150)
		{
			this->level++;
		}
	}

	void GameState::renderGui()
	{
		this->_data->window.draw(this->pointText);
		this->_data->window.draw(this->playerHpBarBack);
		this->_data->window.draw(this->playerHpBar);
		this->_data->window.draw(this->PauseText);
	}

	void GameState::renderWorld()
	{
		this->_data->window.draw(this->worldBackground);
	}

	void GameState::updateStatus()
	{
		if (this->_homeButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(this->_data->input.GetMousePosition(this->_data->window))))
			this->_homeButtonStatus = ButtonStatus::hover;
		else
			this->_homeButtonStatus = ButtonStatus::idle;

		if (this->_homeButtonStatus == ButtonStatus::hover)
			this->_homeButton.setColor(sf::Color::Green);

		if (this->_homeButtonStatus == ButtonStatus::idle)
			this->_homeButton.setColor(sf::Color::White);
	}
}