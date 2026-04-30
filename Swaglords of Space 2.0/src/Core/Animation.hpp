#pragma once

#include<SFML/System/Time.hpp>
#include<SFML/System/Clock.hpp>

namespace _Swag {

	class Animation
	{
	public:
		Animation() = default;

		virtual ~Animation() { };

		virtual void Initialize(const sf::Time& duration) { m_End = duration; }

		virtual void Start(const sf::Time& offset = sf::Time::Zero) = 0;
		virtual void Pause() = 0;
		virtual void Resume() = 0;
		virtual void Stop() = 0;

	protected:
		sf::Time m_Start, m_End;
		sf::Clock m_timer;
		float m_speed = 1;

		bool m_isStarted = false;
	};
}