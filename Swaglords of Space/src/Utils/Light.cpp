#include "Light.h"
#include "Core\DEFFINITIONS.h"
#include "Core\log.h"

namespace Swag {

	Light::Light(sf::Color color, const sf::Vector2f& position, const float& power)
		:m_color(color),
		 m_position(position),
		 m_power(power)
	{
		if (!this->m_shader.loadFromFile(LIGHT_VERTEX_SHADER, LIGHT_FRAGMENT_SHADER))
		{
			SWAG_ERROR("Failed to Load the Shader");
			__debugbreak();
		}

		this->m_shader.setUniform("lightPosition", m_position);
		this->m_shader.setUniform("power", m_power);
		this->m_shader.setUniform("lightColor", sf::Glsl::Vec4(m_color));
	}
	Light::~Light()
	{
		
	}
	void Light::Init(sf::Color color, const sf::Vector2f& position, const float& power)
	{
		m_color = color;
		m_position = position;
		m_power = power;
		if (!this->m_shader.loadFromFile(LIGHT_VERTEX_SHADER, LIGHT_FRAGMENT_SHADER))
		{
			SWAG_ERROR("Failed to Load the Shader");
			__debugbreak();
		}

		this->m_shader.setUniform("lightPosition", m_position);
		this->m_shader.setUniform("power", m_power);
		this->m_shader.setUniform("lightColor", sf::Glsl::Vec4(m_color));
	}
	void Light::UpdateColor(const sf::Color& color)
	{
		this->m_color = color;
		this->m_shader.setUniform("lightColor", sf::Glsl::Vec4(m_color));
	}
	void Light::UpdatePower(const float& power)
	{
		this->m_power = power;
		this->m_shader.setUniform("power", m_power);
	}
	void Light::UpdatePosition(sf::Vector2f position)
	{
		this->m_position = position;
		this->m_shader.setUniform("lightPosition", m_position);
	}
}