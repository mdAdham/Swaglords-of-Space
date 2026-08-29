#include "Player.hpp"
#include "Core/Log.hpp"

#include <fstream>
#include <math.h>
#include <glm.hpp>

#include "Core/Deffinitions.hpp"

namespace _Swag {

	void Player::Init(float speed, float attackcooldown, int hpMax, int boostMax, sf::Texture& texture, sf::Vector2f pos)
	{
		this->_movementSpeed = speed;
		this->_attackCooldownMax = attackcooldown;
		this->_attackCooldown = this->_attackCooldownMax;

		this->_hpMax = hpMax;
		this->_hp = this->_hpMax;

		this->_boostMax = boostMax;
		this->_boost = this->_boostMax;

		this->_sprite = CreateRef<sf::Sprite>();

		this->_sprite->setTexture(texture);

		this->_sprite->setOrigin(getBounds().left + getBounds().width / 2, getBounds().top + getBounds().height / 2);

		this->_sprite->setPosition(pos);
		this->_sprite->setScale(sf::Vector2f(0.6f, 0.6f));

		this->_collider = CreateRef<Collider>(this->_sprite);

		m_deathAnimation = CreateRef<PlayerDeathAnimation>();
		m_deathAnimation->Init(*this);

#ifdef F_PLAYER_SAVE_VARIENT_IMAGE

		std::fstream file("Text.dat", std::ios::out);

		if (file.is_open())
		{
			sf::Image img = this->_sprite->getTexture()->copyToImage();
			img.saveToFile("Test2.png");
			for (unsigned y = 0; y < img.getSize().y; y++)
			{
				for (unsigned x = 0; x < img.getSize().x; x++)
				{
					if (img.getPixel(x, y).a > 0)
					{
						img.setPixel(x, y, sf::Color::Transparent);
						file << "1";
					}
					else
					{
						img.setPixel(x, y, sf::Color::White);
						file << " ";
					}	
				}
				file << "\n";
			}
			img.saveToFile("Test.png");
		}
		file.close();
#endif // F_PLAYER_SAVE_VARIENT_IMAGE
	}

	void Player::InitParticleSystem()
	{
		_particleSysEnable = true;
		_particleSystem.Init(getThrusterPos(), 10, sf::Color(190, 50, 50), sf::Vector2f(), sf::Vector2f(), 0.1f, 1.f, 1, 6, false, 0.2f);
	}

	const sf::Vector2f& Player::getPos() const
	{
		return this->_sprite->getPosition();
	}

	const float Player::getRot() const
	{
		return this->_sprite->getRotation();
	}

	const sf::FloatRect Player::getBounds() const
	{
		return this->_sprite->getGlobalBounds();
	}

	const sf::Vector2f Player::getCenter() const
	{
		// Asumming that the Center is the Origin of the Box
		return { getBounds().left + getBounds().width / 2, getBounds().top + getBounds().height / 2 };
	}

	const int& Player::getHp() const
	{
		return this->_hp;
	}

	const int& Player::getHpMax() const
	{
		return this->_hpMax;
	}

	const int& Player::getBoost() const
	{
		return this->_boost;
	}

	const int& Player::getBoostMax() const
	{
		return this->_boostMax;
	}

	const sf::Vector2f Player::getThrusterPos() const
	{
		sf::Vector2f result;

		float angleDegrees = getRot() - 90.f;
		float angleRadiens = glm::radians(angleDegrees);


		sf::Vector2f direction(std::cos(angleRadiens), std::sin(angleRadiens));

		sf::Vector2f ThrusterOffset = direction * (getBounds().height / 2.4f);
		result = getPos() - ThrusterOffset;

		return result;
	}

	void Player::setPosition(const sf::Vector2f pos)
	{
		this->_sprite->setPosition(pos);
	}

	void Player::setPosition(const float x, const float y)
	{
		this->_sprite->setPosition(x, y);
	}

	void Player::setHp(const int hp)
	{
		this->_hp = hp;
	}

	void Player::loseHp(const int value)
	{
		this->_hp -= value;
		if (this->_hp < 0)
			this->_hp = 0;
	}

	void Player::setBoost(const int boost)
	{
		this->_boost = boost;
	}

	void Player::gainBoost(const int value)
	{
		this->_boost += value;
		if (this->_boost > this->_boostMax)
			this->_boost = this->_boostMax;
	}

	void Player::loseBoost(const int value)
	{
		this->_boost -= value;
		if (this->_boost < 0)
		{
			this->_boost = 0;
		}
	}

	void Player::setDt(const float& dt)
	{
		this->dt = dt;
	}

	void Player::move(ParticleSystem* parsys)
	{
		float angleDegrees = getRot() - 90.f;
		float angleRadiens = angleDegrees * 3.14159f / 180.f;

		sf::Vector2f forward(std::cos(angleRadiens), std::sin(angleRadiens));
		sf::Vector2f left(forward.y, -forward.x);
		sf::Vector2f right(-forward.y, forward.x);
		sf::Vector2f backward = -forward;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && getBoost() != 0 || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift) && getBoost() != 0)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				velocity += left * _movementSpeed * 2.0f; loseBoost(1);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				velocity += right * _movementSpeed * 2.0f; loseBoost(1);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				if (parsys)
				{
					parsys->EmitFrom(getThrusterPos(), { 0.0f, 0.0f }, 60, 5, 
						6, 11, -velocity);
				}
				else if (_particleSysEnable)
				{
					_particleSystem.EmitFrom(getThrusterPos(), { 0.0f, 0.0f }, 60, 5,
						6, 11, -velocity, true);
				}

				velocity += forward * _movementSpeed * 2.0f; loseBoost(1);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				velocity += backward * _movementSpeed * 2.0f; loseBoost(1);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				velocity += left * _movementSpeed * 2.0f; loseBoost(1);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				velocity += right * _movementSpeed * 2.0f; loseBoost(1);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				velocity += forward * _movementSpeed * 2.0f; loseBoost(1);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				velocity += backward * _movementSpeed * 2.0f; loseBoost(1);
			}
		}
		else
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				velocity += left * _movementSpeed;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				velocity += right * _movementSpeed;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				velocity += forward * _movementSpeed;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				velocity += backward * _movementSpeed;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				velocity += left * _movementSpeed;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				velocity += right * _movementSpeed;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				velocity += forward * _movementSpeed;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				velocity += backward * _movementSpeed;

			// Rotation
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
				this->_sprite->rotate(-2.0f);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
				this->_sprite->rotate(2.0f);

		}
	}

	void Player::move(const float dirX, const float dirY, bool withmovementspeed)
	{
		if (withmovementspeed)
			this->_sprite->move(this->_movementSpeed * dirX, this->_movementSpeed * dirY);
		else
			this->_sprite->move(dirX, dirY);
	}

	void Player::recoil(sf::Vector2f _recol)
	{
		velocity.x += _recol.x;
		velocity.y += _recol.y;
	}

	void Player::rotate(const float angle)
	{
		this->_sprite->rotate(angle);
	}

	const bool Player::canAttack()
	{
		if (this->_attackCooldown >= this->_attackCooldownMax)
		{
			this->_attackCooldown = 0.f;
			return true;
		}

		return false;
	}

	bool Player::interset(Ref<Collider> other)
	{
		if (_sprite->getGlobalBounds().intersects(other->GetBounds()))
		{
		/*
			sf::Image img = this->_sprite->getTexture()->copyToImage();
		for (unsigned y = 0; y < img.getSize().y; y++)
		{
			for (unsigned x = 0; x < img.getSize().x; x++)
			{
				//_SWAG_DEBUGS("{0}, {1}", x, y);
				if (img.getPixel(x, y).a == 0)
				{
					continue;
				}
				else
				{
					if (box.contains(_sprite.getGlobalBounds().left + x, _sprite.getGlobalBounds().top + y))
						return true;
				}
			}
		}*/

			return _collider->IsCollide(other);
		}
		return false;
	}

	void Player::update()
	{
		if (this->_attackCooldown < this->_attackCooldownMax)
			this->_attackCooldown += 0.5f;

		_collider->UpdateBounds(getBounds());

		//float x = velocity.x > 0.0f ? std::clamp<float>(velocity.x, 0.0f, 500.0f) : std::clamp<float>(velocity.x, -500.0f, 0.0f);
		//float y = velocity.y > 0.0f ? std::clamp<float>(velocity.y, 0.0f, 500.0f) : std::clamp<float>(velocity.y, -500.0f, 0.0f);
		
		this->_sprite->move(sf::Vector2f(velocity.x, velocity.y) * dt);

		if (velocity.x > 0.0f)
			velocity.x *= AIR_DRAG;
		else if (velocity.x < 0.0f)
			velocity.x *= AIR_DRAG;
		if (velocity.y > 0.0f)
			velocity.y *= AIR_DRAG;
		else if (velocity.y < 0.0f)
			velocity.y *= AIR_DRAG;

		if (std::abs(velocity.x) < 0.5f)
			velocity.x = 0.f;
		if (std::abs(velocity.y) < 0.5f)
			velocity.y = 0.f;

		if (_particleSysEnable)
			_particleSystem.Update(dt, sf::Vector2f());
	}

	void Player::render(sf::RenderTarget& target)
	{
		if (_particleSysEnable)
			_particleSystem.Draw(target);

		target.draw(*this->_sprite);

	}
	void Player::PlayDeathAnimation(sf::RenderTarget& target)
	{
		if (_hp <= 0)
			this->_isPlayerDead = true;

		if (this->_isPlayerDead)
		{
			m_deathAnimation->Start();
			m_deathAnimation->Update(dt);
			m_deathAnimation->Render(target);
		}
	}


	void PlayerDeathAnimation::Init(Player& player)
	{
		m_player = player;
		Animation::Initialize(sf::seconds(5.f));

		m_DeathParticleSystem.Init(player.getPos(), 500, sf::Color(200, 100, 90), sf::Vector2f(0.0f, 0.0f), sf::Vector2f(0.0f, 0.0f),
			0.5f, 1.5f, 3 * glm::length(glm::vec2(player._sprite->getScale().x, player._sprite->getScale().y)), 4 * glm::length(glm::vec2(player._sprite->getScale().x, player._sprite->getScale().y)),
			false, 0.1f);
	}
	void PlayerDeathAnimation::Start(const sf::Time& offset)
	{
		if (m_isStarted == true)
			return;

		m_isFinished = false;
		m_isStarted = true;

		m_DeathParticleSystem.EmitFrom(m_player.getPos(), sf::Vector2f(0.0f, 0.0f), 360, 500, 30.f, 50.f, sf::Vector2f(1.0f, 1.0f), true, sf::Color(255, 255, 255), 
			3 * glm::length(glm::vec2(m_player._sprite->getScale().x, m_player._sprite->getScale().y)), 4 * glm::length(glm::vec2(m_player._sprite->getScale().x, m_player._sprite->getScale().y)));
	}
	void PlayerDeathAnimation::Pause()
	{

	}
	void PlayerDeathAnimation::Resume()
	{

	}
	void PlayerDeathAnimation::Stop()
	{
		
	}
	void PlayerDeathAnimation::Update(float dt)
	{
		if (m_isStarted && !m_DeathParticleSystem.isEmpty())
		{
			this->m_DeathParticleSystem.Update(dt, sf::Vector2f(0.0f, 0.0f));
			//_SWAG_TRACE("Player Death Animation Updating!!");
		}
	}
	void PlayerDeathAnimation::Render(sf::RenderTarget& target)
	{
		if (m_isStarted && !m_DeathParticleSystem.isEmpty())
		{
			this->m_DeathParticleSystem.Draw(target);
			//_SWAG_TRACE("Player Death Animation Rendering!!");
		}
	}
}