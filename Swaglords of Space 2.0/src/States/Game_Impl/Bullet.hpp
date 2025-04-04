#pragma once

#include <SFML/Graphics.hpp>

namespace _Swag {
	class Bullet
	{
	public:
		Bullet() = delete;
		Bullet(sf::Texture& texture, float posX, float posY, float dir_X, float dir_Y, float movement_speed);
		~Bullet() = default;

		const sf::FloatRect getBounds() const;

		void update(float dt);
		void render(sf::RenderTarget* target) const;
	private:
		sf::Sprite _shape;

		sf::Vector2f _direction;
		float _movementSpeed;
	};
}