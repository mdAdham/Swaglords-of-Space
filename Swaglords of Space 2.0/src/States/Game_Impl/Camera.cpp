#include "Camera.hpp"

namespace _Swag {
	Camera::Camera(const sf::Vector2f& center, const sf::Vector2f& size, const float& zoom)
		:zoom(1)
	{
		this->_view.setCenter(center);
		this->_view.setSize(size);
		this->_view.zoom(zoom);

		this->_smoothing = 0.5f;
	}
	void Camera::Update(Ref<Player> player)
	{
		//Position
		const sf::Vector2f target_Position = player->getPos();
		const sf::Vector2f current_Center = _view.getCenter();

		sf::Vector2f newCenter = current_Center + (target_Position - current_Center) * _smoothing;

		_view.setCenter(newCenter);

		//Rotation
		const float target_Angle = player->getRot();
		const float current_Rotation = _view.getRotation();

		float angleDiff = target_Angle - current_Rotation;

		//Shortest Rotation direction
		if (angleDiff > 180.f)
			angleDiff -= 360.f;
		else if (angleDiff < -180.f)
			angleDiff += 360.f;
		
		float newRotation = current_Rotation + angleDiff * _smoothing;
		
		_view.setRotation(newRotation);
	}
	void Camera::Zoom(float delta)
	{
		if(delta < 0.0f)
			zoom = 0.5;
		if (delta > 0.0f)
			zoom = 1.5f;

		this->_view.zoom(zoom);
	}
	void Camera::StartCameraRegion(sf::RenderWindow& window) const
	{
		window.setView(this->_view);
	}
	void Camera::EndCameraRegion(sf::RenderWindow& window)
	{
		window.setView(window.getDefaultView());
	}
}