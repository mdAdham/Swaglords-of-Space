#include "Enemy.h"

void Enemy::initVariables()
{
	this->pointCount = rand() % 8 + 3; //min = 3, max = 10
	this->type = 0;
	this->speed = static_cast<float>(this->pointCount / 2);
	this->hpMax = static_cast<int>(this->pointCount);
	this->hp = this->hpMax;
	this->damage = this->pointCount;
	this->points = this->pointCount;

	//this->light.Init(sf::Color::White, this->shape.getPosition(), this->damage * 200);
}

void Enemy::initShape()
{
	this->shape.setRadius(static_cast<float>(this->pointCount) * 5);
	this->shape.setPointCount(static_cast<size_t>(this->pointCount));
	this->shape.setFillColor(sf::Color(rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1, 255));
}

Enemy::Enemy(float pos_x, float pos_y)
{
	this->initVariables();
	this->initShape();

	this->shape.setPosition(pos_x, pos_y);
}

Enemy::~Enemy()
{
	
}
//Accessors
const sf::FloatRect Enemy::getBounds() const
{
	return this->shape.getGlobalBounds();
}

const int& Enemy::getPoints() const
{
	return this->points;
}

const int& Enemy::getDamage() const
{
	return this->damage;
}

//Functions
void Enemy::update()
{
	this->shape.move(0.f, this->speed);
	//this->light.UpdateColor(this->shape.getFillColor());
	//this->light.UpdatePosition(this->shape.getPosition());
	//this->light.UpdatePower(this->damage * 500);
}

void Enemy::render(sf::RenderTarget* target, sf::Shader& h)
{
	target->draw(this->shape);
}
