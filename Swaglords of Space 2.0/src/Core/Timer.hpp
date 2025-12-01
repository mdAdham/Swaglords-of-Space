#pragma once

#include <SFML/System.hpp>

namespace _Swag {

	class Timer
	{
	public:
		Timer();
	
		sf::Time ElapsedTime();
		sf::Time Restart();
	private:
		sf::Clock clock;
	};

}
