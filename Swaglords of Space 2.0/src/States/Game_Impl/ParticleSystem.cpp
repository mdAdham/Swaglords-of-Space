#include "ParticleSystem.hpp"

#include <random>
#include <algorithm>
#include <cmath>
#include <limits>
#include <execution>

#include <glm.hpp>

namespace _Swag {
	
	
	ParticleSystem::ParticleSystem()
	{
		std::random_device rd;
		m_rng.seed(rd());
	
		m_normalLineBuff.setPrimitiveType(sf::Lines);
		m_normalLineBuff.setUsage(sf::VertexBuffer::Stream);
	
		m_normalLineBuff.create(2);
	}
	
	void ParticleSystem::Init(const sf::Vector2f& pos, int count, const sf::Color& color, const sf::Vector2f& initial_velocity_min,
		const sf::Vector2f& initial_velocity_max, const float& duration_min, const float& duration_max,
		const int& size_min, const int& size_max, bool enable, float airDrag)
	{
			// Store parameters for possible later Emit
		m_initPos = pos;
		m_initColor = color;
		m_velMin = initial_velocity_min;
		m_velMax = initial_velocity_max;
		m_durMin = duration_min;
		m_durMax = duration_max;
		m_sizeMin = size_min;
		m_sizeMax = size_max;
		m_enabled = enable;
		m_airDrag = airDrag;
	
		if (!enable || count <= 0)
		{
			m_particles.clear();
			return;
		}
	
		// Ensure valid ranges
		float dmin = std::min(duration_min, duration_max);
		float dmax = std::max(duration_min, duration_max);
		if (dmin == dmax) dmax = dmin + std::numeric_limits<float>::epsilon();
	
		int smin = std::min(size_min, size_max);
		int smax = std::max(size_min, size_max);
		if (smin < 1) smin = 1;
		if (smax < smin) smax = smin;
	
		float vxmin = std::min(initial_velocity_min.x, initial_velocity_max.x);
		float vxmax = std::max(initial_velocity_min.x, initial_velocity_max.x);
		float vymin = std::min(initial_velocity_min.y, initial_velocity_max.y);
		float vymax = std::max(initial_velocity_min.y, initial_velocity_max.y);
	
		// Prepare distributions
		std::uniform_real_distribution<float> durationDist(dmin, dmax);
		std::uniform_int_distribution<int> sizeDist(smin, smax);
		std::uniform_int_distribution<int> pointDist(3, 9);
		std::uniform_real_distribution<float> velXDist(vxmin, vxmax);
		std::uniform_real_distribution<float> velYDist(vymin, vymax);
	
		m_particles.clear();
		m_particles.reserve(std::max(0, count));
		for (int i = 0; i < count; ++i)
		{
			int radius = sizeDist(m_rng);
			int pointcount = pointDist(m_rng);
			float duration = durationDist(m_rng);
	
			sf::CircleShape shape(static_cast<float>(radius), static_cast<std::size_t>(pointcount));
			shape.setFillColor(color);
			shape.setPosition(pos);
			shape.setOrigin(static_cast<float>(radius), static_cast<float>(radius)); // center origin
	
			Particle p;
			p.duration = duration;
			p.age = 0.f;
			p.velocity = { velXDist(m_rng), velYDist(m_rng) };
			// mass proportional to area (approx) to make collisions feel natural
			p.mass = std::max(0.01f, 3.14159265f * radius * radius * 0.001f);
	
			m_particles.emplace_back(std::move(shape), p);
		}
	}
	
	void ParticleSystem::Emit(int count)
	{
		if (!m_enabled || count <= 0) return;
	
		// Reuse Init's stored ranges to emit additional particles at stored pos
		Init(m_initPos, count, m_initColor, m_velMin, m_velMax, m_durMin, m_durMax, m_sizeMin, m_sizeMax, true, m_airDrag);
		//EmitFrom(m_initPos, { 0.0f, 0.0f }, 360, count, );
	}
	
	void ParticleSystem::EmitFrom(const sf::Vector2f& pos, const sf::Vector2f& direction, float coneAngleDeg,
		int count, float speedMin, float speedMax, const sf::Vector2f& emitterVelocity, bool enabled, const sf::Color& color,
		float sizeMin, float sizeMax)
	{
		m_enabled = enabled;
	
		if (!m_enabled || count <= 0) return;
	
		if (color == sf::Color::Transparent);
		else
			m_initColor = color;
	
		if (sizeMin == 0 && sizeMax == 0);
		else
		{
			m_sizeMin = sizeMin;
			m_sizeMax = sizeMax;
		}
	
		//Init(pos, count, m_initColor, -emitterVelocity * speedMin, -emitterVelocity * speedMax, m_durMin, m_durMax, m_sizeMin, m_sizeMax, true, m_airDrag);
	
		float coneAngleRad = glm::radians(coneAngleDeg);
	
		// Prepare distributions
		std::uniform_real_distribution<float> durationDist(m_durMin, m_durMax);
		std::uniform_int_distribution<int> sizeDist(m_sizeMin, m_sizeMax);
		std::uniform_int_distribution<int> pointDist(3, 9);
		std::uniform_real_distribution<float> angleDist(-coneAngleRad, coneAngleRad);
		std::uniform_real_distribution<float> speedDist(speedMin, speedMax);
	
		for (int i = 0; i < count; i++)
		{
			int radius = sizeDist(m_rng);
			int pointcount = pointDist(m_rng);
			float duration = durationDist(m_rng);
	
			sf::CircleShape shape(static_cast<float>(radius), static_cast<size_t>(pointcount));
	
			shape.setFillColor(m_initColor);
			shape.setOrigin(static_cast<float>(radius), static_cast<float>(radius));
			shape.setPosition(pos);
	
			Particle p;
			p.duration = duration;
			p.age = 0.0f;
			p.mass = std::max(0.01f, 3.14159265f * radius * radius * 0.001f);
	
	
			{
				// For the Emmision Cone - (-coneAngleRad, coneAngleRad)
	
				glm::vec2 emmiterVel = { emitterVelocity.x, emitterVelocity.y };
				glm::vec2 normalVec = glm::normalize(emmiterVel);
	
				float angle = angleDist(m_rng);
				float cosA = std::cos(angle);
				float sinA = std::sin(angle);
	
				glm::vec2 rotatedNormal = {
					normalVec.x * cosA - normalVec.y * sinA,
					normalVec.x * sinA + normalVec.y * cosA
				};
	
				float speed = speedDist(m_rng);
	
				p.velocity = { rotatedNormal.x * speed * 10, rotatedNormal.y * speed * 10};
				//p.velocity = emitterVelocity;
			}
	
	
	
			m_particles.emplace_back(std::move(shape), p);
		}
	}
	
	void ParticleSystem::Update(float dt, const sf::Vector2f& globalForce)
	{
		if (dt <= 0.f || m_particles.empty()) return;
	
		// Simple integration: apply force, update velocity and position, age particles
		for (auto& pair : m_particles)
		{
			Particle& p = pair.second;
			// acceleration = F / m
			sf::Vector2f accel = { globalForce.x / p.mass, globalForce.y / p.mass };
			p.velocity += accel * dt;
	
			pair.first.move(p.velocity * dt);
			p.age += dt;
			p.velocity -= p.velocity * m_airDrag * dt;
		}
	
		// Handle pairwise collisions (naive O(n^2))
		const float restitution = 0.75f; // how bouncy collisions are (0..1)
		const float percent = 0.2f;      // positional correction percentage
		const float slop = 0.01f;        // overlap allowance
	
		for (std::size_t i = 0; i < m_particles.size(); ++i)
		{
			for (std::size_t j = i + 1; j < m_particles.size(); ++j)
			{
				auto& A = m_particles[i];
				auto& B = m_particles[j];
	
				sf::Vector2f posA = A.first.getPosition();
				sf::Vector2f posB = B.first.getPosition();
	
				float rA = A.first.getRadius();
				float rB = B.first.getRadius();
	
				sf::Vector2f n = { posA.x - posB.x, posA.y - posB.y };
				float dist2 = n.x * n.x + n.y * n.y;
				float radii = rA + rB;
	
				if (dist2 <= 0.f)
				{
					// perfectly overlapping; n is undefined - nudge slightly
					n = { 0.01f, 0.01f };
					dist2 = n.x * n.x + n.y * n.y;
				}
	
				float dist = std::sqrt(dist2);
				float overlap = radii - dist;
				if (overlap > 0.f)
				{
					// normalize n
					sf::Vector2f normal = { n.x / dist, n.y / dist };
	
					// positional correction to avoid sinking
					float correction = std::max(overlap - slop, 0.0f) / (1.0f / A.second.mass + 1.0f / B.second.mass) * percent;
					sf::Vector2f correctionVec = { correction * normal.x, correction * normal.y };
					// move A by +correction * (1/mA) and B by -correction * (1/mB)
					A.first.move(correctionVec.x * (1.0f / A.second.mass), correctionVec.y * (1.0f / A.second.mass));
					B.first.move(-correctionVec.x * (1.0f / B.second.mass), -correctionVec.y * (1.0f / B.second.mass));
	
					// relative velocity
					sf::Vector2f rv = { A.second.velocity.x - B.second.velocity.x, A.second.velocity.y - B.second.velocity.y };
					float velAlongNormal = rv.x * normal.x + rv.y * normal.y;
	
					// Do not resolve if velocities are separating
					if (velAlongNormal > 0) continue;
	
					// calculate impulse scalar
					float invMassA = 1.0f / A.second.mass;
					float invMassB = 1.0f / B.second.mass;
	
					float k = -(1.0f + restitution) * velAlongNormal;
					k /= (invMassA + invMassB);
	
					sf::Vector2f impulse = { k * normal.x, k * normal.y };
	
					A.second.velocity.x += impulse.x * invMassA;
					A.second.velocity.y += impulse.y * invMassA;
					B.second.velocity.x -= impulse.x * invMassB;
					B.second.velocity.y -= impulse.y * invMassB;
				}
			}
		}
	
		// Remove dead particles (age >= duration)
		std::vector<std::pair<sf::CircleShape, Particle>> alive;
		alive.reserve(m_particles.size());
		for (auto& pr : m_particles)
		{
			if (pr.second.age < pr.second.duration)
				alive.emplace_back(std::move(pr));
		}
		m_particles.swap(alive);
	
	}
	
	void ParticleSystem::Draw(sf::RenderTarget& target) const
	{
		for (const auto& pr : m_particles)
			target.draw(pr.first);
	}
	
	void ParticleSystem::Clear()
	{
		m_particles.clear();
	}
	
	void ParticleSystem::DrawNormalLine(const sf::Vector2f& origin, const sf::Vector2f& norm, sf::RenderTarget& target)
	{
		m_normal = norm;
		m_normal.x *= 100;
		m_normal.y *= 100;
	
		m_normalBufferVector[0].position = origin;
		m_normalBufferVector[1].position = origin + m_normal;
		m_normalLineBuff.update(m_normalBufferVector, 2, 0);
	
		target.draw(m_normalLineBuff);
	}
	
	void ParticleSystem::Follow(const sf::Vector2f& pos, sf::RenderTarget& target)
	{
		sf::Vector2f cvec = sf::Vector2f(pos.x - m_initPos.x, pos.y - m_initPos.y);
	
		sf::Vector2f normal = sf::Vector2f(cvec.x / sqrt(pow(cvec.x, 2) + pow(cvec.y, 2)),
			cvec.y / sqrt(pow(cvec.x, 2) + pow(cvec.y, 2)));
	
		for (auto& particle : m_particles)
		{
	
			cvec = sf::Vector2f(pos.x - particle.first.getPosition().x,
				pos.y - particle.first.getPosition().y);
	
			float magnitude = (float)sqrt(pow(cvec.x, 2) + pow(cvec.y, 2));
	
			normal = sf::Vector2f(cvec.x / magnitude,
				cvec.y / magnitude);
	
			/*
				float attenuation = min(1.0 / ((constant + linear_constant *
									distance + quadratic_constant * (distance * distance))),
									1.0
									);
			*/
	
			{
	
				float linearConstant = 0.14;
				float quadraticConstant = 0.07;
	
				float attenuation = std::min(1.0 / ((1 + linearConstant *
					magnitude + quadraticConstant * (magnitude * magnitude))),
					1.0
				);
	
				magnitude *= attenuation;
			}
			magnitude *= 10;
	
			particle.first.move({ normal.x * 1 * magnitude, normal.y * 1 * magnitude });
	
			//DrawNormalLine(particle.first.getPosition(), normal, target);
		}
	}
	bool ParticleSystem::isEmpty()
	{
		return this->m_particles.empty();
	}
}