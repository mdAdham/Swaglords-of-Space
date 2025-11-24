#pragma once

#include <SFML/Graphics/View.hpp>

#include "Core/Core.hpp"
#include "Player.hpp"

namespace _Swag
{
	class Camera
	{
	public:
		Camera(const sf::Vector2f& center, const sf::Vector2f& size, const float& zoom);

		void Update(Ref<Player> player);
		void Zoom(float delta);

		void StartCameraRegion(sf::RenderWindow& window) const;
		void EndCameraRegion(sf::RenderWindow& window);

	private:
		sf::View _view;
		float zoom;
		float _smoothing;
	};
}