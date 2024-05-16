#pragma once

#include <SFML/Window/Event.hpp>

namespace _Swag
{
	class State
	{
	public:
		virtual void Init() = 0;

		virtual void OnEvent(sf::Event& ev) = 0;
		virtual void OnUpdate(float dt) = 0;
		virtual void OnRender(float dt) = 0;

		virtual void OnDestroy() { }

		virtual void OnPause() { }
		virtual void OnResume() { }
	};
}