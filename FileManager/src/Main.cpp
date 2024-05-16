#include <SFML/Graphics.hpp>

#include "gui.hpp"

#define NOMINMAX
#include <Windows.h>



int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "FileManager-BackupStudio", sf::Style::Close | sf::Style::Titlebar);
	sf::Event ev;

	window.setFramerateLimit(90);

	ImGui::SFML::Init(window);

	Gui gui(&window);

	sf::Clock deltaClock;
	while (window.isOpen())
	{
		while (window.pollEvent(ev))
		{
			ImGui::SFML::ProcessEvent(ev);
			if (ev.type == sf::Event::Closed)
				window.close();
		}
		ImGui::SFML::Update(window, deltaClock.restart());

		window.clear();
		
		gui.Run();

		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();
	return 0;
}