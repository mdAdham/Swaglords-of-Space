#pragma once
#include "State_Impl/State.hpp"
#include "Core/Core.hpp"
#include "Core/AppData.hpp"

#include "Gui_Impl/Gui.hpp"

namespace _Swag {
	class DifficultyLevelState : public State
	{
	public:
		DifficultyLevelState(Ref<GameData> data);
		~DifficultyLevelState();

		// Inherited via State
		void Init() override;

		void OnEvent(sf::Event& ev) override;

		void OnUpdate(float dt) override;

		void OnRender(float dt) override;
	private:
		Ref<GameData> _data;

		sf::Text _diffText{};
		Ref<_gui::DropDownList> _difficultyLevelSelector;
		std::string _difflevellist[4] = { "American", "Intermediate", "Pro", "Indian"};

		Ref<_gui::Button> _Play_Btn;

		//Ref<_gui::RadialButton> musicbtn;

		sf::VideoMode vm;
	};
}