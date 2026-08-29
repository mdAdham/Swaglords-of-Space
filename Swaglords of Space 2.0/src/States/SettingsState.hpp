#pragma once
#include "State_Impl/State.hpp"
#include "Core/Core.hpp"
#include "Core/AppData.hpp"

#include "Gui_Impl/Gui.hpp"

namespace _Swag {
	class SettingsState : public State
	{
	public:
		SettingsState(Ref<GameData> data);
		~SettingsState();

		void Init() override;

		void OnEvent(sf::Event& ev) override;

		void OnUpdate(float dt) override;

		void OnRender(float dt) override;
	private:
		Ref<GameData> _data;

		sf::VideoMode vm;
	};
}