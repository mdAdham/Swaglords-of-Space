#include "Bullet.hpp"

namespace _Swag {
	Bullet::Bullet(sf::Texture& texture, float posX, float posY, float dir_X, float dir_Y, float movement_speed)
	{
		this->_shape.setTexture(texture);
		this->_shape.setPosition(posX, posY);
		this->_shape.setScale(0.1f, 0.1f);

		this->_direction.x = dir_X;
		this->_direction.y = dir_Y;
		this->_movementSpeed = movement_speed;
	}

	const sf::FloatRect Bullet::getBounds() const
	{
		return this->_shape.getGlobalBounds();
	}


	void Bullet::update(float dt)
	{
		//Movement
		this->_shape.move(this->_movementSpeed * this->_direction * dt);
	}

	void Bullet::render(sf::RenderTarget* target) const
	{
		target->draw(this->_shape);
	}
}