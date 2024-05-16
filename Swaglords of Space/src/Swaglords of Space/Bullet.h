#pragma once

#include<SFML/Graphics.hpp>

class Bullet
{
private:

	sf::Sprite shape;
	sf::Texture* texture;

	sf::Vector2f direction;
	float movementSpeed;


public:
	Bullet() = delete;
	Bullet(sf::Texture* texture, float posX, float posY, float dir_X, float dir_Y, float movement_speed);
	virtual ~Bullet();

	//Accessor
	const sf::FloatRect getBounds() const;

	void update();
	void render(sf::RenderTarget* target) const;

protected:
};

