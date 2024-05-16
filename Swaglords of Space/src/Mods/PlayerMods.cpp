#include "PlayerMods.h"
#include <string>
#include "Core/log.h"

namespace Swag
{
	void PlayerMod::Init()
	{
		if (this->modstate)
		{
			this->modFile.open("Resources/Data/playerData.swagmod", std::ios::in);
			if (this->modFile.is_open())
			{
				std::string line[4];
				int count = 0;
				while (std::getline(this->modFile, line[count]))
				{
					count++;
				}

				this->data.playerSpeed = stof(line[0]);
				this->data.playerAttackCollDownMax = stof(line[1]);
				this->data.playerHpMax = stoi(line[2]);
				this->data._boostmax = stoi(line[3]);
				count = 0;
			}
			else
			{
				this->data = playerData();
			}
			this->modFile.close();
		}
		else
		{
			this->data = playerData();
		}
	}
}