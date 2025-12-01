#pragma once

#include <SFML/Graphics.hpp>

namespace _Swag {
	class Bullet
	{
	public:
		Bullet() = delete;
		Bullet(sf::Texture& texture, float posX, float posY, float dir_X, float dir_Y, float movement_speed, float lifetime);
		~Bullet() = default;

		const sf::FloatRect getBounds() const;
		const bool isAlive();
		void rotate(const float& rotate);

		void update(float dt);
		void render(sf::RenderTarget* target) const;
	private:
		sf::Sprite _shape;

		sf::Vector2f _direction;
		float _movementSpeed;
		float lifetime = 0;
		float lifetimeCounter = 0;
		float dt;
	};
}