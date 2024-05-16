#include "InputManager.hpp"

namespace _Swag
{
	bool InputManager::isSpriteClicked(sf::Sprite object, sf::Mouse::Button button, sf::RenderWindow& window)
	{
		if (sf::Mouse::isButtonPressed(button))
		{
			sf::IntRect tempRect(object.getPosition().x, object.getPosition().y, object.getGlobalBounds().width, object.getGlobalBounds().height);

			if (tempRect.contains(sf::Mouse::getPosition(window)))
			{
				return true;
			}
		}

		return false;
	}

	bool InputManager::isTextClicked(sf::Text object, sf::Mouse::Button button, sf::RenderWindow& window)
	{
		if (sf::Mouse::isButtonPressed(button))
		{
			sf::IntRect tempRect(object.getPosition().x, object.getPosition().y, object.getGlobalBounds().width, object.getGlobalBounds().height);

			if (tempRect.contains(sf::Mouse::getPosition(window)))
			{
				return true;
			}
		}

		return false;
	}

	sf::Vector2i InputManager::GetMousePosition(const sf::RenderWindow& window)
	{
		//return sf::vector2i
		return sf::Mouse::getPosition(window);
	}

	sf::Vector2i InputManager::GetMousePosition()
	{
		return sf::Mouse::getPosition();
	}

	sf::Vector2f InputManager::GetMousePositionRelToView(const sf::RenderWindow& window)
	{
		return window.mapPixelToCoords(GetMousePosition(window));
	}
}