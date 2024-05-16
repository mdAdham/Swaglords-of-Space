#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Core/Core.hpp"

namespace _Swag {
	
	class Collider
	{
	public:
		Collider(sf::Sprite& spr);
	
		void UpdateBounds(sf::FloatRect rect);
		bool IsCollide(const Ref<Collider>& another);
		const sf::FloatRect& GetBounds();

		void Render(sf::RenderWindow& window);
	private:
		sf::FloatRect bounds;
		float top = 0, bottom = 0, left = 0, right = 0;
		float int_top = 0, int_bottom = 0, int_left = 0, int_right = 0;
		std::vector<std::vector<bool>> mask;
	};
}