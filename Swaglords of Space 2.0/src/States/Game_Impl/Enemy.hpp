#pragma once

#include <SFML/Graphics.hpp>
#include "Colliders.hpp"
#include "Core/Core.hpp"

namespace _Swag {
	class Enemy
	{
	public:
		Enemy(float pos_s, float pos_y, float enemy_damage_factor, float enemy_speed_factor, float enemy_points_factor);
		~Enemy() = default;

		//Accessors
		const sf::FloatRect getBounds() const;
		const int& getPoints() const;
		const int getDamage() const;

		//Functions
		void update(float dt);
		void render(sf::RenderTarget* target) const;
		Ref<Collider> _collider;
	private:
		unsigned pointCount = 0;
		sf::CircleShape shape;
		Ref<sf::Sprite> _sprite;
		float speed = 0;
		unsigned int damage = 0;
		int playerpoints = 0;
	};
}