#include "Timer.hpp"

namespace _Swag {

	Timer::Timer()
	{
		clock.restart();
	}

	sf::Time Timer::ElapsedTime()
	{
		return clock.getElapsedTime();
	}

	sf::Time Timer::Restart()
	{
		return clock.restart();
	}

}