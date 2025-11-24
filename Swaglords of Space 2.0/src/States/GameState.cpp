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
		_data->assets.LoadTexture("Background_Texture", BACKGROUND_TEXTURE, false);

		_background.setTexture(_data->assets.GetTexture("Background_Texture"));
		_background.setOrigin(_background.getGlobalBounds().width / 2, _background.getGlobalBounds().height / 2);
		_background.setPosition(0, 0);
		_background.setScale(10.f, 10.f);
		

		_Player = CreateRef<Player>();

		_Player->Init(_modes.player_speed, _modes.player_attack_cooldown_max, _modes.player_max_hp, _modes.player_max_boost, _data->assets.GetTexture("Ship_Texture"), sf::Vector2f(_gui::p2pX(50, vm), _gui::p2pY(50, vm)));

		_playerHpBar = CreateRef<_gui::ProgressBar>(.8f, 4.0f, 20.0f, 2.0f, sf::Color::Red, 200u, vm, &_data->assets.GetFont("Arial_Font"));
		_playerBoostBar = CreateRef<_gui::ProgressBar>(.8f, 7.0f, 17.0f, 2.0f, sf::Color::Blue, 200u, vm, &_data->assets.GetFont("Arial_Font"));

		this->spawnerTimerMax = _modes.enemie_spawner_Time_Max;
		this->spawnerTimer = this->spawnerTimerMax;

		_Camera = CreateRef<Camera>(_Player->getPos(), sf::Vector2f(WINDOW_WIDHT, WINDOW_HEIGHT), 1.0f);
	}
	void GameState::OnEvent(sf::Event& ev)
	{
		if (ev.type == ev.MouseWheelScrolled)
		{
			_Camera->Zoom(ev.mouseWheelScroll.delta);
		}
	}

	void GameState::OnUpdate(float dt)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
			_SWAG_CRITICAL("Number of Enemies: {0}", this->_enemies.size());

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

				if (_bullets[i]->getBounds().top + _bullets[i]->getBounds().height <= 0.f)
				{
					this->_bullets[i].~shared_ptr();
					this->_bullets.erase(this->_bullets.begin() + i);
				}
			}

			//UPDATE ENEMIE
			this->spawnerTimer += 0.5f;
			if (this->spawnerTimer >= this->spawnerTimerMax)
			{
				this->_enemies.push_back(CreateRef<Enemy>(
					rand() % this->_data->window.getSize().x + 10.f, -100.0f,
					_modes.enemy_damage_factor,
					_modes.enemie_speed_factor,
					static_cast<float>(_modes.enemy_points_factor)
				));
				this->spawnerTimer = 0.f;
			}

			for (unsigned i = 0; i < this->_enemies.size(); i++)
			{
				_enemies[i]->update(dt);
				_enemies[i]->follow(this->_Player);

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
		_Camera->EndCameraRegion(_data->window);

		_playerHpBar->render(_data->window);
		_playerBoostBar->render(_data->window);


		_data->window.display();
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
			float angleRadiens = angleDegrees * 3.14159f / 180.f;

			sf::Vector2f direction(std::cos(angleRadiens), std::sin(angleRadiens));

			sf::Vector2f muzzleOffset = direction * this->_Player->getBounds().height / 2.f;
			sf::Vector2f bulletStartingPos = _Player->getPos() + muzzleOffset;

			auto& bullet = this->_bullets.emplace_back(CreateRef<Bullet>(
				_data->assets.GetTexture("Bullet_Texture"),
				bulletStartingPos.x,
				bulletStartingPos.y,
				direction.x,
				direction.y,
				_modes.bullet_speed
			));
			
			bullet->rotate(_Player->getRot());

			sf::Vector2f recoil = -direction;
			this->_Player->move(recoil.x, recoil.y, false);
		}
	}
}