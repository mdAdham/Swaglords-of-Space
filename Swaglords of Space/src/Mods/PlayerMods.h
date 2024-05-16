#pragma once
#include "Mods.h"

namespace Swag
{
	class PlayerMod : public Mods
	{
	public:
		~PlayerMod() = default;
		void Init();
		void ActivativeMod(bool activate)
		{
			this->modstate = activate;
		}
	private:
		std::fstream modFile;

		struct playerData
		{
			float playerSpeed = 4.05f;
			float playerAttackCollDownMax = 10.0f;
			int playerHpMax = 100;
			int _boostmax;
		};
		bool modstate = false;
	public:
		playerData data;
	};
		
}