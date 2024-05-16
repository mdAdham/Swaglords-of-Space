#pragma once

#include <fstream>

namespace Swag {

	class Mods
	{
	public:
		Mods() = default;
		virtual ~Mods() = default;

		virtual void Init() = 0;
		virtual void ActivativeMod(bool activate) = 0;
	
	};
}