#pragma once

#include <SFML/Graphics.hpp>
#include "Colliders.hpp"
#include "Core/Core.hpp"

namespace _Swag {
	class Player
	{
	public:
		Player() = default;
		~Player() = default;
		
		void Init(float speed, float attackcooldown, int hpMax, int boostMax, sf::Texture& texture, sf::Vector2f pos);

		const sf::Vector2f& getPos() const;
		const sf::FloatRect getBounds() const;
		const sf::Vector2f& getCenter() const;
		const int& getHp() const;
		const int& getHpMax() const;
		const int& getBoost() const;
		const int& getBoostMax() const;

		void setPosition(const sf::Vector2f pos);
		void setPosition(const float x, const float y);
		void setHp(const int hp);
		void loseHp(const int value);
		void setBoost(const int boost);
		void gainBoost(const int value);
		void loseBoost(const int value);

		void move(const float dirX, const float dirY);
		void move(const float dirX, const float dirY, bool withmovementspeed);
		const bool canAttack();
		bool interset(Ref<Collider> other);

		void update();
		void render(sf::RenderTarget& target);

		Ref<Collider> _collider;
	private:
		sf::Sprite _sprite;

		float _movementSpeed = 0.f;

		float _attackCooldown = 0.f;
		float _attackCooldownMax = 0.f;

		int _hp = 0;
		int _hpMax = 0;

		int _boost = 0;
		int _boostMax = 0;
	};
}