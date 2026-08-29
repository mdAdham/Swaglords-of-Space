#pragma once

#include <SFML/Graphics.hpp>
#include "Colliders.hpp"
#include "Core/Core.hpp"


#include "Core/Animation.hpp"
#include "ParticleSystem.hpp"

#include "Core/AppData.hpp"

namespace _Swag {

	class PlayerDeathAnimation;

	class Player
	{
	public:
		Player() = default;
		~Player() = default;

		Player(Ref<GameData> data);
		
		void Init(float speed, float attackcooldown, int hpMax, int boostMax, sf::Texture& texture, sf::Vector2f pos);
		void InitParticleSystem();

		const sf::Vector2f& getPos() const;
		const float getRot() const;
		const sf::FloatRect getBounds() const;
		// Returns the Global Origin of the player
		const sf::Vector2f getCenter() const;
		const int& getHp() const;
		const int& getHpMax() const;
		const int& getBoost() const;
		const int& getBoostMax() const;
		const sf::Vector2f getThrusterPos() const;
		const sf::Vector3f getUpVector() const;
		const sf::Vector3f getDirection() const;

		void setPosition(const sf::Vector2f pos);
		void setPosition(const float x, const float y);
		void setHp(const int hp);
		void loseHp(const int value);
		void setBoost(const int boost);
		void gainBoost(const int value);
		void loseBoost(const int value);
		void setDt(const float& dt);

		void move(ParticleSystem* parsys = nullptr);
		void move(const float dirX, const float dirY, bool withmovementspeed);
		void recoil(sf::Vector2f);

		void rotate(const float angle);

		const bool canAttack();
		bool interset(Ref<Collider> other);

		void update();
		void render(sf::RenderTarget& target);

		void PlayDeathAnimation(sf::RenderTarget& target);

		Ref<Collider> _collider;
	private:
		Ref<GameData> _data;
		Ref<sf::Sprite> _sprite;
		ParticleSystem _particleSystem;
		bool _particleSysEnable = false;
		
		float _movementSpeed = 0.f;

		float _attackCooldown = 0.f;
		float _attackCooldownMax = 0.f;

		int _hp = 0;
		int _hpMax = 0;

		int _boost = 0;
		int _boostMax = 0;

		float dt = 0.f;

		bool _isPlayerDead = false;

		Ref<PlayerDeathAnimation> m_deathAnimation;
	public:
		sf::Vector2f velocity;

		friend class PlayerDeathAnimation;
	};


	class PlayerDeathAnimation : public Animation
	{
	public:
		PlayerDeathAnimation() = default;
		~PlayerDeathAnimation() = default;

		void Init(Player& player);
		void Start(const sf::Time& offset = sf::Time::Zero) override;
		void Pause() override;
		void Resume() override;
		void Stop() override;

		void Update(float dt);
		void Render(sf::RenderTarget& target);

	private:
		ParticleSystem m_DeathParticleSystem;
		Player m_player;

		bool m_isFinished = false;

	};
}