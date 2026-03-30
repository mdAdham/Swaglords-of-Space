#pragma once

#include <vector>
#include <random>
#include <SFML/Graphics.hpp>

class ParticleSystem
{
public:
	ParticleSystem();

	// Initializes/creates `count` particles at `pos`. Keeps original signature.
	void Init(const sf::Vector2f& pos, int count, const sf::Color& color, const sf::Vector2f& initial_velocity_min,
		const sf::Vector2f& initial_velocity_max, const float& duration_min, const float& duration_max,
		const int& size_min, const int& size_max, bool enable, float airDrag);

	// Emit additional particles using the same parameters already configured (optional).
	void Emit(int count);

	void EmitFrom(const sf::Vector2f& pos,
		const sf::Vector2f & direction,
		float coneAngleDeg,
		int count,
		float speedMin,
		float speedMax,
		const sf::Vector2f & emitterVelocity = { 0.f, 0.f });

	// Update all particles: dt in seconds, globalForce applied to each particle (e.g., gravity).
	void Update(float dt, const sf::Vector2f& globalForce);

	// Draw particles to the provided render target.
	void Draw(sf::RenderTarget& target) const;

	// Remove all particles.
	void Clear();

	void Follow(const sf::Vector2f& pos, sf::RenderTarget& target);

	//DEBUG
	void DrawNormalLine(const sf::Vector2f& origin, const sf::Vector2f& norm, sf::RenderTarget& target);

private:
	struct Particle
	{
		float duration = 1.f;          // total life in seconds
		float age = 0.f;               // current age in seconds
		sf::Vector2f velocity = {0.f, 0.f};
		float mass = 1.f;
	};

private:
	// Container of shapes paired with their simulation data
	std::vector<std::pair<sf::CircleShape, Particle>> m_particles;

	// Last initialization parameters (used by Emit)
public:
	sf::Vector2f m_initPos = {0.f, 0.f};
private:
	sf::Color m_initColor = sf::Color::White;
	sf::Vector2f m_velMin = {0.f, 0.f};
	sf::Vector2f m_velMax = {0.f, 0.f};
	float m_durMin = 1.f;
	float m_durMax = 1.f;
	int m_sizeMin = 1;
	int m_sizeMax = 1;
	bool m_enabled = false;
	float m_airDrag = 0;

	// Random generator used for emissions
	std::mt19937 m_rng;

private:
	// Debug
	sf::Vector2f m_normal;
	sf::VertexBuffer m_normalLineBuff;
	sf::Vertex m_normalBufferVector[2];
};