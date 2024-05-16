#pragma once

#include <State/State.h>
#include "Game.h"
#include "Core/Core.hpp"

namespace Swag
{
	class NewGameState : public State
	{
	public:
		NewGameState(GameDataRef state);

		void Init();

		void HandleInput();
		void Update(float dt);
		void Draw(float dt);

	private:
		GameDataRef _data;
	};
}