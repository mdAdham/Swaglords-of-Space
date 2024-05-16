#include "Bullet.h"

Bullet::Bullet()
{

}

Bullet::Bullet(sf::Texture* texture, float pos_X, float pos_Y, float dir_X, float dir_Y, float movement_speed)
{
	this->shape.setTexture(*texture);
	this->shape.setPosition(pos_X, pos_Y);
	this->shape.setScale(0.1f, 0.1f);

	this->direction.x = dir_X;
	this->direction.y = dir_Y;
	this->movementSpeed = movement_speed;
}

Bullet::~Bullet()
{

}

const sf::FloatRect Bullet::getBounds() const
{
	return this->shape.getGlobalBounds();
}


void Bullet::update()
{
	//Movement
	this->shape.move(this->movementSpeed * this->direction);
}

void Bullet::render(sf::RenderTarget* target) const
{
	target->draw(this->shape);
}
