#pragma once

#include <SFML/Graphics.hpp>
#include "Colliders.hpp"
#include "Core/Core.hpp"
#include "Player.hpp"
#include <glm.hpp>


namespace _Swag {
	class Enemy
	{
	public:
		Enemy(float pos_x, float pos_y, float enemy_damage_factor, float enemy_speed_factor, float enemy_points_factor, float lifetime);
		~Enemy() = default;

		//Accessors
		const sf::FloatRect getBounds() const;
		const int& getPoints() const;
		const int getDamage() const;
		const int getPointCount() const;
		const sf::Color getColor() const;
		const float getRadius() const;
		const sf::Vector2f getPos() const;
		const bool isAlive();

		//Functions
		void update(float dt);
		void follow(const Ref<Player>& player);
		void render(sf::RenderTarget* target);
		Ref<Collider> _collider;
		sf::Shader _glowShader{};
	private:
		unsigned pointCount = 0;
		sf::CircleShape shape;
		Ref<sf::Sprite> _sprite;
		float speed = 0;
		unsigned int damage = 0;
		int playerpoints = 0;
		float dt = 0;
		float lifetime = 0;
		float lifetimeCounter = 0;
	};
}