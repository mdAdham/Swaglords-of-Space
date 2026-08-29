#include "Camera.hpp"

namespace _Swag {
	Camera::Camera(const sf::Vector2f& center, const sf::Vector2f& size, const float& zoom, const float& zoommin, const float& zoommax)
		:zoom(1), _zoomMin(zoommin), _zoomMax(zoommax)
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
		if (delta < 0.0f)
		{
			if (_zoomCounter < _zoomMin) { zoom = 1.f; }
			else
				zoom = 0.5;
		}

		if (delta > 0.0f)
		{
			if (_zoomCounter > _zoomMax) { zoom = 1.f; }
			else
				zoom = 1.5f;
		}

		_zoomCounter *= zoom;

		this->_view.zoom(zoom);
	}
	void Camera::StartCameraRegion(sf::RenderTarget& window) const
	{
		window.setView(this->_view);
	}
	void Camera::EndCameraRegion(sf::RenderTarget& window)
	{
		window.setView(window.getDefaultView());
	}
}