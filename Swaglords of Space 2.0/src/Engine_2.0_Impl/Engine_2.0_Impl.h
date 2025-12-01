#pragma once

#include "Core/AppData.hpp"
#include "Core/Deffinitions.hpp"

namespace _Swag::Engine_2 {
	class Engine_v2_Impl
	{
	public:
		Engine_v2_Impl();
		~Engine_v2_Impl();

		void Initialize();

		void Run();

	private:
		Ref<GameData> _data = CreateRef<GameData>();
		sf::Event ev{};

		const float dt = 1.0f / FRAMERATELIMIT;
		sf::Clock dtClock;
		float fps = 0;
	private:
		//Default
		sf::Text _defaulttext;
		sf::Text _fpstext;

		sf::Text _versionoftheeng{};
		void DefaultEvent() const;
		void DefaultRender();
	};
}