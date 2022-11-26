#pragma once

#include <SFML/Graphics.hpp>

#include "State/State.h"
#include "Game.h"

#include "Manager/FileManager.h"

namespace Swag
{
	class LevelState : public State
	{
	public:
		LevelState(GameDataRef data);

		void Init();

		void HandleInput();
		void Update(float dt);
		void Draw(float dt);

	private:
		GameDataRef _data;

		int _level = 0;
		FileManager _levels;
	private:
		enum class ButtonStatus
		{
			idle,
			hover,
			click
		};

	private:
		void UpdateLevels();

	};
}