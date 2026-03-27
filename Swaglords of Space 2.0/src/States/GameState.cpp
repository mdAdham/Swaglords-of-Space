#include "GameState.hpp"
#include "Core/Deffinitions.hpp"
#include <glm.hpp>
#include <gtc/random.hpp>
#include <gtc/constants.hpp>

#include <iostream>

namespace _Swag {
	GameState::GameState(Ref<GameData> data, GameModes modes)
		: _data(data), _modes(modes)
	{
		_SWAG_INFO("GameState Created!");
	}
	void GameState::Init()
	{
		_SWAG_INFO("GameState Initialized!");
		sf::VideoMode vm = sf::VideoMode(_data->window.getSize().x, _data->window.getSize().y);

		_data->assets.LoadTexture("Bullet_Texture", BULLET_TEXTURE, true);
		_data->assets.LoadTexture("Background_Texture", BACKGROUND_TEXTURE, false);

		_data->assets.GetTexture("Background_Texture").setSmooth(true);
		_data->assets.GetTexture("Background_Texture").generateMipmap();
		_background.setTexture(_data->assets.GetTexture("Background_Texture"));
		_background.setOrigin(_background.getGlobalBounds().width / 2, _background.getGlobalBounds().height / 2);
		_background.setPosition(0, 0);
		_background.setScale(20.f, 20.f);
		

		_Player = CreateRef<Player>();

		_Player->Init(_modes.player_speed, _modes.player_attack_cooldown_max, _modes.player_max_hp, _modes.player_max_boost, _data->assets.GetTexture("Ship_Texture"), sf::Vector2f(_gui::p2pX(50, vm), _gui::p2pY(50, vm)));

		_playerHpBar = CreateRef<_gui::ProgressBar>(.8f, 4.0f, 20.0f, 2.0f, sf::Color::Red, 200u, vm, &_data->assets.GetFont("Arial_Font"));
		_playerBoostBar = CreateRef<_gui::ProgressBar>(.8f, 7.0f, 17.0f, 2.0f, sf::Color::Blue, 200u, vm, &_data->assets.GetFont("Arial_Font"));

		this->spawnerTimerMax = _modes.enemie_spawner_Time_Max;
		this->spawnerTimer = this->spawnerTimerMax;

		this->boostIncrementTimerMax = _modes.player_boost_cooldown_max;
		this->boostIncrementTimer = boostIncrementTimerMax;

		_Camera = CreateRef<Camera>(_Player->getPos(), sf::Vector2f(WINDOW_WIDHT, WINDOW_HEIGHT), 1.0f);

		_particleSystem.Init({ 192, 108 }, 100, sf::Color::Blue, { 20.f, 20.f }, {30.f, 30.f}, 20, 25, 5, 20, true);
	}
	void GameState::OnEvent(sf::Event& ev)
	{
		if (ev.type == ev.MouseWheelScrolled)
		{
			_Camera->Zoom(ev.mouseWheelScroll.delta);
		}

		if (ev.key.code == sf::Keyboard::P)
		{
			sf::Vector2f pos = sf::Vector2f(_Player->getPos().x, _Player->getPos().y);

			_particleSystem.EmitFrom(pos, sf::Vector2f(0.f, 0.f), 30, 1000, 5, 10, _Player->velocity);
		}
	}

	void GameState::OnUpdate(float dt)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
			_SWAG_CRITICAL("Number of Enemies: {0}", this->_enemies.size());

		_particleSystem.Update(dt, {0,0});

		if (this->_Player->getHp() != 0)
		{
			this->_Player->setDt(dt);
			//PLAYER MOVEMENT
			this->_Player->move();
			//Update View
			_Camera->Update(this->_Player);

			//Shooting Firing
			UpdateBullets();

			this->_Player->update();

			UpdateGui();

			for (unsigned i = 0; i < _bullets.size(); i++)
			{
				_bullets[i]->update(dt);

				if (!_bullets[i]->isAlive())
				{
					this->_bullets[i].~shared_ptr();
					this->_bullets.erase(this->_bullets.begin() + i);
					continue;
				}
				/*
				if (_bullets[i]->getBounds().top + _bullets[i]->getBounds().height <= 0.f)
				{
					this->_bullets[i].~shared_ptr();
					this->_bullets.erase(this->_bullets.begin() + i);
				}
				*/
			}

			//UPDATE ENEMIE
			this->spawnerTimer += 0.5f;
			if (this->spawnerTimer >= this->spawnerTimerMax)
			{
				SpawnEnemy();
				this->spawnerTimer = 0.f;
			}

			this->boostIncrementTimer += dt;
			if (this->boostIncrementTimer >= this->boostIncrementTimerMax)
			{
				this->_Player->gainBoost(20);
				this->boostIncrementTimer = 0;
			}

#define NORMAL_FOR 0
#if NORMAL_FOR
			// NORMAL FOR
			for (size_t i = 0; i < this->_enemies.size(); i++)
			{
				_enemies[i]->update(dt);
				_enemies[i]->follow(this->_Player);

				//DELETING ENEMY IF THE DIE
				if (!_enemies[i]->isAlive())
				{
					_enemies[i].~shared_ptr();
					_enemies.erase(_enemies.begin() + i);
					continue;
				}

				// Enemy Player Intersect
				else if (this->_Player->interset(_enemies[i]->_collider) == true)
				{
					this->_Player->loseHp(_enemies[i]->getDamage());
					this->_enemies[i].~shared_ptr();
					this->_enemies.erase(this->_enemies.begin() + i);
					//Playes the break sonud
					//_SWAG_TRACE("Enemy deleted!");
					continue;
				}

				///////////////////////////////////Pixel Perfect///////////////////////////////////////////////

				//ENEMY BULLET COLLISION
				bool enemy_deleted = false;
				for (size_t j = 0; j < this->_bullets.size(); j++)
				{
					if (i == this->_enemies.size())
					{
						_SWAG_CRITICAL("THIS MUST BE NOT POSSIBLE:: i == enemies.size() <- Something is wrong");
						continue;
					}

					if (this->_enemies[i]->getBounds().intersects(this->_bullets.at(j)->getBounds()))
					{
						this->points += this->_enemies[i]->getPoints();

						this->_Player->gainBoost(this->_enemies[i]->getDamage());

						this->_enemies[i].~shared_ptr();
						this->_enemies.erase(this->_enemies.begin() + i);

						this->_bullets[j].~shared_ptr();
						this->_bullets.erase(this->_bullets.begin() + j);


						enemy_deleted = true;

						//Play the break sound!
					}
				}
			}

#else
			// NEW FOR

			_deadEnemyIndicies.clear();
			_deadBulletIndicies.clear();

			for (size_t i = 0; i < this->_enemies.size(); i++)
			{
				_enemies[i]->update(dt);
				_enemies[i]->follow(this->_Player);

				if (!_enemies[i]->isAlive())
				{
					_deadEnemyIndicies.push_back(_enemies.begin() + i);
					continue;
				}

				else if (_Player->interset(_enemies[i]->_collider) == true)
				{
					_Player->loseHp(_enemies[i]->getDamage());
					_deadEnemyIndicies.push_back(_enemies.begin() + i);
					continue;
				}

				// Enemy Bullet Collision
				for (size_t j = 0; j < this->_bullets.size(); j++)
				{
					if (i == this->_enemies.size())
					{
						_SWAG_CRITICAL("THIS MUST BE NOT POSSIBLE:: i == enemies.size() <- Something is wrong");
						continue;
					}

					if (this->_enemies[i]->getBounds().intersects(this->_bullets.at(j)->getBounds()))
					{
						this->points += this->_enemies[i]->getPoints();

						this->_Player->gainBoost(this->_enemies[i]->getDamage());

						this->_deadEnemyIndicies.push_back(this->_enemies.begin() + i);
						this->_deadBulletIndicies.push_back(this->_bullets.begin() + j);

						// Play the Break Sound
					}
				}
			}

			for (auto& index : _deadEnemyIndicies)
			{
				_enemies.erase(index);
			}
			for (auto& index : _deadBulletIndicies)
			{
				_bullets.erase(index);
			}
#endif
			//_SWAG_TRACE("No of Enimies: {0}, No of Bullets: {1}", _enemies.size(), _bullets.size());
		}
		else
		{
			if (this->allenemiedeleted == false)
			{
				for (auto& i : this->_bullets)
				{
					i.~shared_ptr();
				}

				for (auto& i : this->_enemies)
				{
					i.~shared_ptr();
				}
				this->allenemiedeleted = true;
			}

			UpdateGui();
		}
	}

	void GameState::OnRender(float dt)
	{

		_Camera->StartCameraRegion(_data->window);
		_data->window.draw(_background);
		if (this->_Player->getHp() > 0)
		{
			_Player->render(_data->window);
			_Player->_collider->Render(_data->window);

			/*sf::RectangleShape rect;
			rect.setPosition(_Player->_collider->GetBounds().left, _Player->_collider->GetBounds().top);
			rect.setSize(sf::Vector2f(_Player->_collider->GetBounds().width, _Player->_collider->GetBounds().height));
			_data->window.draw(rect);*/
		}

		if (this->_Player->getHp() != 0)
		{
			for (auto& bullet : _bullets)
			{
				bullet->render(&_data->window);
			}

			for (auto& enemy : this->_enemies)
			{
				enemy->render(&_data->window);
				/*sf::RectangleShape rect;
				rect.setPosition(enemy->_collider->GetBounds().left, enemy->_collider->GetBounds().top);
				rect.setSize(sf::Vector2f(enemy->_collider->GetBounds().width, enemy->_collider->GetBounds().height));
				_data->window.draw(rect);*/
			}
		}
		_particleSystem.Draw(_data->window);
		_particleSystem.Follow(_Player->getPos(), _data->window);

		_Camera->EndCameraRegion(_data->window);

		_playerHpBar->render(_data->window);
		_playerBoostBar->render(_data->window);

	}

	void GameState::UpdateGui()
	{
		_playerHpBar->update(_Player->getHp(), _Player->getHpMax());
		_playerBoostBar->update(_Player->getBoost(), _Player->getBoostMax());
	}

	void GameState::UpdateBullets()
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->_Player->canAttack() || sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && this->_Player->canAttack())
		{
			float angleDegrees = _Player->getRot() - 90.f;
			float angleRadiens = glm::radians(angleDegrees);


			sf::Vector2f direction(std::cos(angleRadiens), std::sin(angleRadiens));

			sf::Vector2f muzzleOffset = direction * (this->_Player->getBounds().height / 2.f);
			sf::Vector2f bulletStartingPos = _Player->getPos() + muzzleOffset;

			auto& bullet = this->_bullets.emplace_back(CreateRef<Bullet>(
				_data->assets.GetTexture("Bullet_Texture"),
				bulletStartingPos.x,
				bulletStartingPos.y,
				direction.x,
				direction.y,
				_modes.bullet_speed,
				5.f
			));
			
			bullet->rotate(_Player->getRot());

			sf::Vector2f recoil = -direction * 2.f;
			this->_Player->recoil(recoil);
		}
	}
	void GameState::SpawnEnemy()
	{
		glm::vec2 playerPos = { this->_Player->getPos().x, this->_Player->getPos().y };

		float angle = glm::linearRand(0.0f, glm::two_pi<float>());
		float radius = glm::linearRand(3000.f, 4000.f);

		glm::vec2 spawnOffset = {
			glm::cos(angle) * radius,
			glm::sin(angle) * radius
		};

		glm::vec2 enemyPos = {
			playerPos.x + spawnOffset.x,
			playerPos.y + spawnOffset.y
		};

		//_SWAG_TRACE(playerPos.s);

		this->_enemies.push_back(CreateRef<Enemy>(
			enemyPos.x, enemyPos.y,
			_modes.enemy_damage_factor,
			_modes.enemie_speed_factor,
			static_cast<float>(_modes.enemy_points_factor),
			_modes.enemy_lifetime //seconds
		));
	}
}