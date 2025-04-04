#include "Player.h"
#include "Manager/AssetManager.h"

#include "Core/DEFFINITIONS.h"

void Player::initVariables()
{
	this->movememtSpeed = (double)this->mod.data.playerSpeed;

	this->attackColldownMax = (double)this->mod.data.playerAttackCollDownMax;
	this->attackCooldown = this->attackColldownMax;

	this->hpMax = (double)this->mod.data.playerHpMax;
	this->hp = this->hpMax;

	this->boostMax = this->mod.data._boostmax;
	this->boost = this->boostMax;
}

void Player::initTexture()
{
	sf::Image img = Asset::DatToImg(PLAYER_TEXTURE);

	//Load Texture from file
	if (!texture.loadFromImage(img))
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

void Player::initMod()
{
	this->mod.Init();
}

Player::Player()
{
	this->mod.ActivativeMod(true);

	this->initMod();
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

const sf::Vector2f& Player::getCenter() const
{
	sf::Vector2f result(getBounds().left + getBounds().width / 2, getBounds().top + getBounds().height / 2);
	return result;
}

const int& Player::getHp() const
{
	return this->hp;
}

const int& Player::getHpMax() const
{
	return this->hpMax;
}

const int& Player::getBoost() const
{
	return this->boost;
}

const int& Player::getBoostMax() const
{
	return this->boostMax;
}

//Modifiers
void Player::setShader(sf::Shader& shader)
{
	//shader.setUniform("texture", &texture);
}

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

void Player::setBoost(const int boost)
{
	this->boost = boost;
}

void Player::gainBoost(const int value)
{
	this->boost += value;
	if (this->boost > this->boostMax)
		this->boost = this->boostMax;
}

void Player::loseBoost(const int value)
{
	this->boost -= value;
	if (this->boost < 0)
	{
		this->boost = 0;
	}
}

void Player::Rotate(float angle)
{
	this->sprite.rotate(angle);
}

//Functions
void Player::move(const float dirX, const float dirY)
{
	this->sprite.move(this->movememtSpeed * dirX, this->movememtSpeed * dirY);
}

void Player::move(const float dirX, const float dirY, bool withmovementspeed)
{
	if (withmovementspeed)
		this->sprite.move(this->movememtSpeed * dirX, this->movememtSpeed * dirY);
	else
		this->sprite.move(dirX, dirY);
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
	{
		this->attackCooldown += 0.5f;
	}
}

void Player::update()
{
	this->updateAttack();
}

void Player::render(sf::RenderTarget& target, sf::Shader& shader)
{
	target.draw(this->sprite);
}
