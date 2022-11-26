#include "Player.h"

void Player::initVariables()
{
	this->movememtSpeed = 3.f;

	this->attackColldownMax = 10.f;
	this->attackCooldown = this->attackColldownMax;

	this->hpMax = 100;
	this->hp = this->hpMax;
}

void Player::initTexture()
{
	//Load Texture from file
	if (!texture.loadFromFile("Resources/res/Ship.png"))
	{
		std::cout << "ERROR::PLAYER::INITTEXTRUE::Could not load texture file." << std::endl;
	}

}

void Player::initSprite()
{
	//set the texture to the sprite
	this->sprite.setTexture(this->texture);

	//Resize the sprite
	sprite.setPosition(sf::Vector2f(850, 540));
	sprite.setScale(sf::Vector2f(0.6f, 0.6f));
}

Player::Player()
{

	this->initVariables();
	this->initTexture();
	this->initSprite();
}

Player::~Player()
{

}

//Accessors
const sf::Vector2f& Player::getPos() const
{
	return this->sprite.getPosition();
}

const sf::FloatRect Player::getBounds() const
{
	return this->sprite.getGlobalBounds();
}

const int& Player::getHp() const
{
	return this->hp;
}

const int& Player::getHpMax() const
{
	return this->hpMax;
}

//Modifiers
void Player::setPosition(const sf::Vector2f pos)
{
	this->sprite.setPosition(pos);
}

void Player::setPosition(const float x, const float y)
{
	this->sprite.setPosition(x, y);
}

void Player::setHp(const int hp)
{
	this->hp = hp;
}

void Player::loseHp(const int value)
{
	this->hp -= value;
	if (this->hp < 0)
		this->hp = 0;
}

//Functions
void Player::move(const float dirX, const float dirY)
{
	this->sprite.move(this->movememtSpeed * dirX, this->movememtSpeed * dirY);
}

const bool Player::canAttack()
{
	if (this->attackCooldown >= this->attackColldownMax)
	{
		this->attackCooldown = 0.f;
		return true;
	}

	return false;
}

void Player::updateAttack()
{
	if (this->attackCooldown < this->attackColldownMax)
		this->attackCooldown += 0.5f;
}

void Player::update()
{
	this->updateAttack();
}

void Player::render(sf::RenderTarget& target)
{
	target.draw(this->sprite);
}
