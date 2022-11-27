#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>

int main()
{
	sf::RenderWindow window;
	window.create(sf::VideoMode(800, 600), "Swaglords Launcher", sf::Style::Close);
	window.setFramerateLimit(122);
	sf::Event ev{};

	sf::Font font;
	sf::Text title;
	sf::Text launchText;

	sf::Music music;
	music.openFromFile("Resources/audio/test.ogg");
	music.setLoop(true);

	sf::RectangleShape launchButt;

	font.loadFromFile("Resources/fonts/ALGER.ttf");
	title.setFont(font);
	title.setString("Swaglords of Space Launcher");
	title.setCharacterSize(40);

	launchButt.setSize(sf::Vector2f(50.f, 20.f));

	launchText.setFont(font);
	launchText.setString("Launch");
	launchText.setCharacterSize(50);
	launchText.setOutlineColor(sf::Color::Red);
	launchText.setOutlineThickness(1.f);

	music.play();

	int x = 0, y = 0;

	while (window.isOpen())
	{
		while (window.pollEvent(ev))
		{
			if (sf::Event::Closed == ev.type)
				window.close();
		}

		window.clear();

		//update
		title.setPosition(sf::Vector2f(90, 90));
		launchText.setPosition(sf::Vector2f(300, 350));
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (launchText.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
			{
				ShellExecuteW(window.getSystemHandle(), 0, L"Swaglords of Space.exe", 0, L"../Swaglords of Space/", 0);
				window.close();
			}
		}

		//draw
		window.draw(title);
		window.draw(launchText);


		window.display();
	}

	return 0;
}