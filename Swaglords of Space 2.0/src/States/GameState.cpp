#include "GameState.hpp"

#include "Core/Deffinitions.hpp"
#include <glm.hpp>
#include <gtc/random.hpp>
#include <gtc/constants.hpp>
#include "Gui_Impl/Gui.hpp"

#include <iostream>
#include <algorithm>

#define RENDER_TEXTURE_RENDERING

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
		this->_gameTextureVM = vm;
#ifdef RENDER_TEXTURE_RENDERING
		//_gameTextureVM.width = _gameTextureVM.width / 1.2;
		//_gameTextureVM.height = _gameTextureVM.height / 1.2;
		_gametexture.create(_gameTextureVM.width, _gameTextureVM.height);
		_gameTextureShader.loadFromFile(GAME_RENDER_TEXTURE_SHADER_F, sf::Shader::Fragment);
#endif
		
		_data->assets.LoadTexture("Bullet_Texture", BULLET_TEXTURE, true);
		_data->assets.LoadTexture("Background_Texture", BACKGROUND_TEXTURE, false);

		_data->assets.GetTexture("Background_Texture").setSmooth(true);
		_data->assets.GetTexture("Background_Texture").generateMipmap();
		_background.setTexture(_data->assets.GetTexture("Background_Texture"));
		_background.setOrigin(_background.getGlobalBounds().width / 2, _background.getGlobalBounds().height / 2);
		_background.setPosition(0, 0);
		_background.setScale(20.f, 20.f);
		

		_Player = CreateRef<Player>(_data);

		_Player->Init(_modes.player_speed, _modes.player_attack_cooldown_max, 
			_modes.player_max_hp, _modes.player_max_boost, 
			_data->assets.GetTexture("Ship_Texture"), 
			sf::Vector2f(_gui::p2pX(50, _gameTextureVM), _gui::p2pY(50, _gameTextureVM)));

		_Player->InitParticleSystem();

		_playerHpBar = CreateRef<_gui::ProgressBar>(.8f, 4.0f, 20.0f, 2.0f, sf::Color::Red, 200u, _gameTextureVM, &_data->assets.GetFont("Arial_Font"));
		_playerBoostBar = CreateRef<_gui::ProgressBar>(.8f, 7.0f, 17.0f, 2.0f, sf::Color::Blue, 200u, _gameTextureVM, &_data->assets.GetFont("Arial_Font"));

		this->spawnerTimerMax = _modes.enemie_spawner_Time_Max;
		this->spawnerTimer = this->spawnerTimerMax;

		this->boostIncrementTimerMax = _modes.player_boost_cooldown_max;
		this->boostIncrementTimer = boostIncrementTimerMax;

		//_Camera = CreateRef<Camera>(_Player->getPos(), sf::Vector2f(WINDOW_WIDHT, WINDOW_HEIGHT), 1.0f, 0.5, 2.25);
		_Camera = CreateRef<Camera>(_Player->getPos(), sf::Vector2f(_gameTextureVM.width, _gameTextureVM.height), 1.0f, 0.5, 2.25);

		_particleSystem.Init({ 192, 108 }, 100, sf::Color::Blue, { 20.f, 20.f }, {30.f, 30.f}, 5.f, 10.f, 1, 5, true, 0.3f);
		_enemyDeathParticleSystem.Init({ 0.0f, 0.0f }, 10, sf::Color::White, sf::Vector2f(), sf::Vector2f(), 0.1f, 1.0f, 1, 2, false, 0.3f);

		this->pointsText.setFont(_data->assets.GetFont("Arial_Font"));

		this->pointsText.setOrigin(pointsText.getGlobalBounds().width/2,
			pointsText.getGlobalBounds().height / 2);

		this->pointsText.setPosition(
			_gui::p2pX(50.f, _gameTextureVM),
			30.f
		);

		this->pointsText.setCharacterSize(_gui::calcCharSize(_gameTextureVM, 64));

		this->_gameDifficultyText.setFont(_data->assets.GetFont("Arial_Font"));
		this->_gameDifficultyText.setCharacterSize(_gui::calcCharSize(vm, 128));
		this->_gameDifficultyText.setPosition(_gui::p2pX(86, vm), _gui::p2pY(2, vm));
		this->_gameDifficultyText.setString("Difficulty: " + _modes.Name);

		_greyScaleShader.loadFromFile(GREY_SCALE_SHADER_V, GREY_SCALE_SHADER_F);
		//_greyScaleShader.loadFromFile(GREY_SCALE_SHADER_F, sf::Shader::Fragment);
		//_greyScaleShader.setUniform("texture", sf::Shader::CurrentTexture);
		InitSounds();
	}

	void GameState::InitSounds()
	{
		_data->assets.LoadSoundBuffer(SBUFFER_B_S1, SOUND_BULLET_SHOOTING1, false);
		_data->assets.LoadSoundBuffer(SBUFFER_B_S2, SOUND_BULLET_SHOOTING2, false);
		_data->assets.LoadSoundBuffer(SBUFFER_B_S3, SOUND_BULLET_SHOOTING3, false);
		_data->assets.LoadSoundBuffer(SBUFFER_Ship, SOUND_ROCKET_LAUNCH, false);
		_data->assets.LoadSoundBuffer(SBUFFER_Rock, SOUND_ROCK_COLLISION, false);

		_data->assets.LoadSound(S_B_S1, _data->assets.GetSoundBuffer(SBUFFER_B_S1), GROUP_SOUND_BULLET);
		_data->assets.LoadSound(S_B_S2, _data->assets.GetSoundBuffer(SBUFFER_B_S2), GROUP_SOUND_BULLET);
		_data->assets.LoadSound(S_B_S3, _data->assets.GetSoundBuffer(SBUFFER_B_S3), GROUP_SOUND_BULLET);
		_data->assets.LoadSound(S_Ship, _data->assets.GetSoundBuffer(SBUFFER_Ship));
		_data->assets.LoadSound(S_Rock, _data->assets.GetSoundBuffer(SBUFFER_Rock));
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
			pos = _Player->getThrusterPos();

			_particleSystem.EmitFrom(pos, sf::Vector2f(0.f, 0.f), 60, 10, 5, 10, -sf::Vector2f( _Player->velocity.x , _Player->velocity.y ));
		}

		if (ev.key.code == sf::Keyboard::Escape)
		{
			_data->quit = true;
		}

		if (ev.key.code == sf::Keyboard::Enter && this->_gameover)
		{
			_data->machine.RemoveState(); // GameState -> Difficulty Level Selector
		}
	}

	void GameState::OnUpdate(float dt)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
			_SWAG_CRITICAL("Number of Enemies: {0}", this->_enemies.size());

		_particleSystem.Update(dt, { 0.f,0.f });
		_enemyDeathParticleSystem.Update(dt, { 0.0f, 0.0f });

		this->_Player->setDt(dt);

		if (this->_Player->getHp() != 0)
		{
			this->_Player->move();

			_data->audio.setListnerPosition(_Player->getPos(), _Player->getUpVector(), _Player->getDirection());
			_Camera->Update(this->_Player);

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
 // This is error
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
					_deadEnemyIndicies.push_back(i);
					continue;
				}

				// Enemy Player Collision
				else if (_Player->interset(_enemies[i]->_collider) == true)
				{
					_Player->loseHp(_enemies[i]->getDamage());
					_deadEnemyIndicies.push_back(i);

					{
						sf::Vector2f emissionVec(1.f, 1.f);

						_enemyDeathParticleSystem.EmitFrom(_Player->_collider->GetIntersectionPoint(), sf::Vector2f(0.0f, 0.0f), 360, _enemies[i]->getPointCount() * 2,
							5.f, 10.f, emissionVec, true, _enemies[i]->getColor(), _enemies[i]->getRadius() / 8, _enemies[i]->getRadius() / 6);
						
						_data->audio.playSFX(S_Rock, _Player->getPos());
					}
					continue;
				}

				// Enemy Bullet Collision
				for (size_t j = 0; j < this->_bullets.size(); j++)
				{
					if (this->_enemies[i]->getBounds().intersects(this->_bullets.at(j)->getBounds()))
					{
						this->points += this->_enemies[i]->getPoints();

						this->_Player->gainBoost(this->_enemies[i]->getDamage());

						this->_deadEnemyIndicies.push_back(i);
						this->_deadBulletIndicies.push_back(j);

						// Play the Particles
						{

							sf::Vector2f enemyPos, emissionVec;

							enemyPos.x = _enemies[i]->getBounds().left + _enemies[i]->getBounds().width;
							enemyPos.y = _enemies[i]->getBounds().top + _enemies[i]->getBounds().height;

							emissionVec.x = _bullets[j]->getDirection().x * 50;
							emissionVec.y = _bullets[j]->getDirection().y * 50;

							_enemyDeathParticleSystem.EmitFrom(enemyPos, sf::Vector2f(0.0f, 0.0f), 360, _enemies[i]->getPointCount() * 2, 5.f, 10.f, emissionVec,
								true, _enemies[i]->getColor(), _enemies[i]->getRadius() / 8, _enemies[i]->getRadius() / 6);
						}

						// Play the Break Sound
						_data->audio.playSFX(S_Rock, _enemies[i]->getPos());
					}
				}
			}

			this->pointsText.setString(std::to_string(points));

			int index = 0;
			int removeindex = 0;

			_enemies.erase(std::remove_if(_enemies.begin(), _enemies.end(),
				[&](const auto&) {
					return std::find(_deadEnemyIndicies.begin(),_deadEnemyIndicies.end(),
						index++) != _deadEnemyIndicies.end();
				}), _enemies.end());

			index = 0;
			removeindex = 0;

			_bullets.erase(std::remove_if(_bullets.begin(), _bullets.end(),
				[&](const auto&) {
					return std::find(_deadBulletIndicies.begin(),
						_deadBulletIndicies.end(),
						index++) != _deadBulletIndicies.end();
				}), _bullets.end());

			_enemyDeathParticleSystem.Update(dt, { 0.0f, 0.0f });
#endif
			//_SWAG_TRACE("No of Enimies: {0}, No of Bullets: {1}", _enemies.size(), _bullets.size());

			_shaderCounter += dt;
			_greyScaleShader.setUniform("wave_phase", _shaderCounter);
			_greyScaleShader.setUniform("wave_amplitude", sf::Glsl::Vec2(10, 10));

		}
		else
		{
			this->_gameover = true;
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

#ifndef RENDER_TEXTURE_RENDERING

	void GameState::OnRender(float dt)
	{
		_Camera->StartCameraRegion(_data->window);
		{
			//_greyScaleShader.setUniform("wave_amplitude", sf::Glsl::Vec2(_Player->getPos()));
			_data->window.draw(_background, &_greyScaleShader);
		}
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
		_enemyDeathParticleSystem.Draw(_data->window);

		this->_Player->PlayDeathAnimation(_data->window);

		//_particleSystem.Follow(_Player->getPos(), _data->window);

		_Camera->EndCameraRegion(_data->window);

		_playerHpBar->render(_data->window);
		_playerBoostBar->render(_data->window);
		_data->window.draw(pointsText);
	}

#else

	void GameState::OnRender(float dt)
	{
		//_Camera->StartCameraRegion(_data->window);
		_gametexture.clear();
		_Camera->StartCameraRegion(_gametexture);
		{
			_gametexture.draw(_background, &_greyScaleShader);
			//_data->window.draw(_background, &_greyScaleShader);
		}
		if (this->_Player->getHp() > 0)
		{
			//_Player->render(_data->window);
			_Player->render(_gametexture);
			//_Player->_collider->Render(_data->window); // Depreciated


			/*sf::RectangleShape rect;
			rect.setPosition(_Player->_collider->GetBounds().left, _Player->_collider->GetBounds().top);
			rect.setSize(sf::Vector2f(_Player->_collider->GetBounds().width, _Player->_collider->GetBounds().height));
			_data->window.draw(rect);*/
		}

		if (this->_Player->getHp() != 0)
		{
			for (auto& bullet : _bullets)
			{
				//bullet->render(&_data->window);
				bullet->render(&_gametexture);
			}

			for (auto& enemy : this->_enemies)
			{
				//enemy->render(&_data->window);
				enemy->render(&_gametexture);


				/*sf::RectangleShape rect;
				rect.setPosition(enemy->_collider->GetBounds().left, enemy->_collider->GetBounds().top);
				rect.setSize(sf::Vector2f(enemy->_collider->GetBounds().width, enemy->_collider->GetBounds().height));
				_data->window.draw(rect);*/
			}
		}
		//_particleSystem.Draw(_data->window);
		//_enemyDeathParticleSystem.Draw(_data->window);
		_particleSystem.Draw(_gametexture);
		_enemyDeathParticleSystem.Draw(_gametexture);

		//this->_Player->PlayDeathAnimation(_data->window);
		this->_Player->PlayDeathAnimation(_gametexture);

		//_particleSystem.Follow(_Player->getPos(), _data->window);

		//_Camera->EndCameraRegion(_data->window);
		_Camera->EndCameraRegion(_gametexture);

		_gametexture.display();
		//_gameTextureShader.setUniform("texture", sf::Shader::CurrentTexture);

		_data->window.draw(sf::Sprite{_gametexture.getTexture()}, &_gameTextureShader);

		_playerHpBar->render(_data->window);
		_playerBoostBar->render(_data->window);
		_data->window.draw(pointsText);
		_data->window.draw(_gameDifficultyText);
	}

#endif
	void GameState::UpdateGui()
	{
		_playerHpBar->update(_Player->getHp(), _Player->getHpMax());
		_playerBoostBar->update(_Player->getBoost(), _Player->getBoostMax());

		this->pointsText.setOrigin(pointsText.getGlobalBounds().width / 2,
			pointsText.getGlobalBounds().height / 2);

		this->pointsText.setPosition(
			_gui::p2pX(50.f, _gameTextureVM),
			30.f
		);

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

			//_data->audio.playSFX(S_B_S1, bullet->getPos());
			_data->audio.playRandomSFX(GROUP_SOUND_BULLET, bullet->getPos());
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