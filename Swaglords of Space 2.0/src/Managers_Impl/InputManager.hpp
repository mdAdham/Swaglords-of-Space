#pragma once

#include <SFML/Graphics.hpp>

namespace _Swag
{
	static class InputManager
	{
	public:
		InputManager() { }
		~InputManager() { }

		static bool isSpriteClicked(sf::Sprite object, sf::Mouse::Button button, sf::RenderWindow& window);
		static bool isTextClicked(sf::Text, sf::Mouse::Button button, sf::RenderWindow& window);

		static sf::Vector2i GetMousePosition(const sf::RenderWindow& window);
		static sf::Vector2i GetMousePosition();
		static sf::Vector2f GetMousePositionRelToView(const sf::RenderWindow& window);
	};
}

