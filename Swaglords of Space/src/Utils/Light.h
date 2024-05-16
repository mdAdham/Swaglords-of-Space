#pragma once

#include <SFML/Graphics/Shader.hpp>
namespace Swag {

	class Light
	{
	public:
		Light() = default;
		Light(sf::Color color, const sf::Vector2f& position, const float& power);
		~Light();

		void Init(sf::Color color, const sf::Vector2f& position, const float& power);

		void UpdateColor(const sf::Color& color);
		void UpdatePower(const float& power);
		void UpdatePosition(sf::Vector2f position);

		//operator sf::Shader*()
		//{
		//	return &this->m_shader;
		//}
		//operator sf::Shader&()
		//{
		//	return this->m_shader;
		//}
		sf::Shader m_shader;
	private:
		sf::Color m_color;
		sf::Vector2f m_position;
		float m_power = 10;
	};

}