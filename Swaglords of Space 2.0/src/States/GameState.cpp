#include "GameState.hpp"
#include "Core/Deffinitions.hpp"

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

		_Player = CreateRef<Player>();

		_Player->Init(_modes.player_speed, _modes.player_attack_cooldown_max, _modes.player_max_hp, _modes.player_max_boost, _data->assets.GetTexture("Ship_Texture"), sf::Vector2f(_gui::p2pX(50, vm), _gui::p2pY(50, vm)));

		_playerHpBar = CreateRef<_gui::ProgressBar>(.8, 4, 20, 2, sf::Color::Red, 200, vm, &_data->assets.GetFont("Arial_Font"));
		_playerBoostBar = CreateRef<_gui::ProgressBar>(.8, 7, 17, 2, sf::Color::Blue, 200, vm, &_data->assets.GetFont("Arial_Font"));

		this->spawnerTimerMax = _modes.enemie_spawner_Time_Max;
		this->spawnerTimer = this->spawnerTimerMax;

		
	}
	void GameState::OnEvent(sf::Event& ev)
	{
		
	}

	void GameState::OnUpdate(float dt)
	{
		if (this->_Player->getHp() != 0)
		{
			//PLAYER MOVEMENT
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && this->_Player->getBoost() != 0 || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift) && this->_Player->getBoost() != 0)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				{
					this->_Player->move(-2.f, 0.f); this->_Player->loseBoost(1);
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				{
					this->_Player->move(2.f, 0.f); this->_Player->loseBoost(1);
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				{
					this->_Player->move(0.f, -2.f); this->_Player->loseBoost(1);
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				{
					this->_Player->move(0.f, 2.f); this->_Player->loseBoost(1);
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				{
					this->_Player->move(-2.f, 0.f); this->_Player->loseBoost(1);
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
					this->_Player->move(2.f, 0.f); this->_Player->loseBoost(1);
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				{
					this->_Player->move(0.f, -2.f); this->_Player->loseBoost(1);
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				{
					this->_Player->move(0.f, 2.f); this->_Player->loseBoost(1);
				}
			}
			else
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
					this->_Player->move(-1.f, 0.f);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
					this->_Player->move(1.f, 0.f);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
					this->_Player->move(0.f, -1.f);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
					this->_Player->move(0.f, 1.f);

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
					this->_Player->move(-1.f, 0.f);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
					this->_Player->move(1.f, 0.f);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
					this->_Player->move(0.f, -1.f);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
					this->_Player->move(0.f, 1.f);
			}


			//Shooting Firing
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->_Player->canAttack() || sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && this->_Player->canAttack())
			{
				this->_bullets.push_back(CreateRef<Bullet>(
					_data->assets.GetTexture("Bullet_Texture"),
					this->_Player->getCenter().x - 5,
					this->_Player->getPos().y,
					0.f,
					-1.f,
					_modes.bullet_speed
				));
				this->_Player->move(0.f, 5.0f, false);
			}

			this->_Player->update();

			UpdateGui();

			for (unsigned i = 0; i < _bullets.size(); i++)
			{
				_bullets[i]->update(dt);

				if (_bullets[i]->getBounds().top + _bullets[i]->getBounds().height <= 0.f)
				{
					this->_bullets[i].~shared_ptr();
					this->_bullets.erase(this->_bullets.begin() + i);
				}
			}

			//PLAYER COLLISION
			
			//LEFT
			if (this->_Player->getBounds().left <= 0.f)
			{
				this->_Player->setPosition(0.f, this->_Player->getBounds().top);
			}
			
			//RIGHT
			else if (this->_Player->getBounds().left + this->_Player->getBounds().width >= this->_data->window.getSize().x)
			{
				this->_Player->setPosition(this->_data->window.getSize().x - this->_Player->getBounds().width, this->_Player->getBounds().top);
			}

			//TOP
			if (this->_Player->getBounds().top <= 0.f)
			{
				this->_Player->setPosition(this->_Player->getBounds().left, 0);
			}

			//BOTTOM
			else if (this->_Player->getBounds().top + this->_Player->getBounds().height >= this->_data->window.getSize().y)
			{
				this->_Player->setPosition(this->_Player->getBounds().left, this->_data->window.getSize().y - this->_Player->getBounds().height);
			}
			//_SWAG_TRACE("Player Position: {0}:{1}", this->_Player->getBounds().left, this->_Player->getBounds().top);

			//UPDATE ENEMIE
			this->spawnerTimer += 0.5f;
			if (this->spawnerTimer >= this->spawnerTimerMax)
			{
				this->_enemies.push_back(CreateRef<Enemy>(
					rand() % this->_data->window.getSize().x + 10.f, -100,
					_modes.enemy_damage_factor,
					_modes.enemie_speed_factor,
					_modes.enemy_points_factor
				));
				this->spawnerTimer = 0.f;
			}

			for (unsigned i = 0; i < this->_enemies.size(); i++)
			{
				_enemies[i]->update(dt);

				// DELETING ENEMY AT THE BOTTOM OF THE SCREEN
				if (_enemies[i]->getBounds().top > this->_data->window.getSize().y)
				{
					_enemies[i].~shared_ptr();
					_enemies.erase(_enemies.begin() + i);
					continue;
				}

				// ENEMY PLAYER COLLISION
				///////////////////////////////////Pixel Perfect///////////////////////////////////////////////
				/*else if (_enemies[i]->getBounds().intersects(this->_Player->getBounds()))
				{
					this->_Player->loseHp(_enemies[i]->getDamage());
					this->_enemies[i].~shared_ptr();
					this->_enemies.erase(this->_enemies.begin() + i);
					//Playes the break sonud
					continue;
				}*/
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
					if (this->_enemies[i]->getBounds().intersects(this->_bullets[j]->getBounds()))
					{
						this->points += this->_enemies[i]->getPoints();

						this->_enemies[i].~shared_ptr();
						this->_enemies.erase(this->_enemies.begin() + i);

						this->_bullets[j].~shared_ptr();
						this->_bullets.erase(this->_bullets.begin() + j);

						enemy_deleted = true;

						//Play the break sound!
					}
				}
			}

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

		_playerHpBar->render(_data->window);
		_playerBoostBar->render(_data->window);

		_data->window.display();
	}

	void GameState::UpdateGui()
	{
		_playerHpBar->update(_Player->getHp(), _Player->getHpMax());
		_playerBoostBar->update(_Player->getBoost(), _Player->getBoostMax());
	}
}