#pragma once

#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<iostream>

#include "Mods/PlayerMods.h"

class Player
{
private:
	sf::Sprite sprite;
	sf::Texture texture;

	float movememtSpeed;

	float attackCooldown;
	float attackColldownMax;

	int hp;
	int hpMax;

	int boost;
	int boostMax;

	Swag::PlayerMod mod;

	//Private Functions
	void initVariables();
	void initTexture();
	void initSprite();
	void initMod();

public:
	Player();
	virtual ~Player();

	//Accessors
	const sf::Vector2f& getPos() const;
	const sf::FloatRect getBounds() const;
	const sf::Vector2f& getCenter() const;
	const int& getHp() const;
	const int& getHpMax() const;
	const int& getBoost() const;
	const int& getBoostMax() const;

	//Modifiers
	void setShader(sf::Shader& shader);
	void setPosition(const sf::Vector2f pos);
	void setPosition(const float x, const float y);
	void setHp(const int hp);
	void loseHp(const int value);
	void setBoost(const int boost);
	void gainBoost(const int value);
	void loseBoost(const int value);
	void Rotate(float angle);

	//Functions
	void move(const float dirX, const float dirY);
	void move(const float dirX, const float dirY, bool withmovementspeed);
	const bool canAttack();

	void updateAttack();
	void update();
	void render(sf::RenderTarget& target, sf::Shader& shader);

};

