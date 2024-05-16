#include "Player.hpp"
#include "Core/Log.hpp"

#include <fstream>

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

		this->_sprite.setTexture(texture);

		this->_sprite.setOrigin(getCenter());

		this->_sprite.setPosition(pos);
		this->_sprite.setScale(sf::Vector2f(0.6f, 0.6f));

		this->_sprite.setOrigin(0, 0);

		this->_collider = CreateRef<Collider>(this->_sprite);

		std::fstream file("Text.dat", std::ios::out);

		if (file.is_open())
		{

		sf::Image img = this->_sprite.getTexture()->copyToImage();
		img.saveToFile("Test2.png");
		for (unsigned x = 0; x < img.getSize().x; x++)
		{
			std::vector<bool> tempmask;
			for (unsigned y = 0; y < img.getSize().y; y++)
			{
				if (img.getPixel(x, y).a > 0)
				{
					img.setPixel(x, y, sf::Color::Transparent);
					file << "1";
					tempmask.push_back(true);
				}
				else
				{
					img.setPixel(x, y, sf::Color::White);
					file << " ";
					tempmask.push_back(false);
				}	
			}
			//mask.push_back(tempmask);
			file << "\n";
		}
		img.saveToFile("Test.png");
		}
		file.close();
	}

	const sf::Vector2f& Player::getPos() const
	{
		return this->_sprite.getPosition();
	}

	const sf::FloatRect Player::getBounds() const
	{
		return this->_sprite.getGlobalBounds();
	}

	const sf::Vector2f& Player::getCenter() const
	{
		sf::Vector2f result(getBounds().left + getBounds().width / 2, getBounds().top + getBounds().height / 2);
		return result;
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

	void Player::setPosition(const sf::Vector2f pos)
	{
		this->_sprite.setPosition(pos);
	}

	void Player::setPosition(const float x, const float y)
	{
		this->_sprite.setPosition(x, y);
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

	void Player::move(const float dirX, const float dirY)
	{
		this->_sprite.move(this->_movementSpeed * dirX, this->_movementSpeed * dirY);
	}

	void Player::move(const float dirX, const float dirY, bool withmovementspeed)
	{
		if (withmovementspeed)
			this->_sprite.move(this->_movementSpeed * dirX, this->_movementSpeed * dirY);
		else
			this->_sprite.move(dirX, dirY);
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
		if (_sprite.getGlobalBounds().intersects(other->GetBounds()))
		{
			sf::Image img = this->_sprite.getTexture()->copyToImage();
		/*
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
	}

	void Player::render(sf::RenderTarget& target)
	{
		target.draw(this->_sprite);
	}
}