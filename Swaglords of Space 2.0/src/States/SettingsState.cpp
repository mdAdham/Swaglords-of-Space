#include "SettingsState.hpp"

namespace _Swag {

	SettingsState::SettingsState(Ref<GameData> data)
		:_data(data)
	{
	}
	SettingsState::~SettingsState()
	{
	}
	void SettingsState::Init()
	{
	}
	void SettingsState::OnEvent(sf::Event& ev)
	{
		if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape)
			_data->machine.RemoveState();
	}
	void SettingsState::OnUpdate(float dt)
	{
	}
	void SettingsState::OnRender(float dt)
	{
	}
}
