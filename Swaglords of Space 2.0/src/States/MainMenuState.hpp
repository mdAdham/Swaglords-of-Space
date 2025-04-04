#pragma once

#include "State_Impl/State.hpp"
#include "Core/Core.hpp"
#include "Core/AppData.hpp"

#include "Gui_Impl/Gui.hpp"

namespace _Swag {

	class MainMenuState : public State
	{
	public:
		MainMenuState(Ref<GameData> data);
		// Inherited via State
		void Init() override;

		void OnEvent(sf::Event& ev) override;

		void OnUpdate(float dt) override;

		void OnRender(float dt) override;

		void OnDestroy() override;

	private:
		Ref<GameData> _data;
		std::map<std::string, Ref<_gui::Button>> _buttons;

		sf::Sprite _ShipLogo{};
		sf::Text _GameName{};
	};

}
