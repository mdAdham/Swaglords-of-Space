#include "Bullet.hpp"

namespace _Swag {
	Bullet::Bullet(sf::Texture& texture, float posX, float posY, float dir_X, float dir_Y, float movement_speed, float lifetime)
		: dt(0)
	{
		this->_shape.setTexture(texture);
		this->_shape.setOrigin(_shape.getGlobalBounds().width / 2, _shape.getGlobalBounds().height);

		this->_shape.setScale(0.1f, 0.1f);
		this->_shape.setPosition(posX, posY);

		this->_direction.x = dir_X;
		this->_direction.y = dir_Y;
		this->_movementSpeed = movement_speed;

		this->lifetime = lifetime;
		this->lifetimeCounter = 0;
	}

	const sf::FloatRect Bullet::getBounds() const
	{
		return this->_shape.getGlobalBounds();
	}

	const sf::Vector2f Bullet::getDirection() const
	{
		return this->_direction;
	}

	const bool Bullet::isAlive()
	{
		this->lifetimeCounter += dt;
		if (this->lifetimeCounter >= this->lifetime)
		{
			this->lifetimeCounter = 0;
			return false;
		}
		return true;
	}

	void Bullet::rotate(const float& rotate)
	{
		this->_shape.rotate(rotate);
	}

	void Bullet::update(float _dt)
	{
		//Movement
		this->dt = _dt;
		this->_shape.move(this->_movementSpeed * this->_direction * this->dt);
	}

	void Bullet::render(sf::RenderTarget* target) const
	{
		target->draw(this->_shape);
	}
}