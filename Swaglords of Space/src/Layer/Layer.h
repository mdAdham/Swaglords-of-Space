#pragma once

namespace Swag
{
	class Layer
	{
	public:
		virtual void Init() = 0;

		virtual void HandleInput() = 0;
		virtual void Update() = 0;
		virtual void Draw() = 0;

		virtual void Resume() { }
		virtual void Pause() { }
	};
}