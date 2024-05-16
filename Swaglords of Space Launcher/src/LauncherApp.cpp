#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>
#include <vector>
#include <fstream>

namespace fs = std::filesystem;

class Launcher {
public:
	Launcher() : window(sf::VideoMode(800, 600), "Game Launcher", sf::Style::Close | sf::Style::Titlebar) {
		scanApplications(); // Scan for metadata files
		createUI(); // Create user interface
	}

	void run() {
		while (window.isOpen()) {
			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					window.close();
				else if (event.type == sf::Event::MouseButtonPressed)
					handleMouseClick(event.mouseButton);
			}

			window.clear(sf::Color(30, 30, 30)); // Set background color
			drawUI();
			window.display();
		}
	}

private:
	sf::RenderWindow window;
	std::vector<std::pair<std::string, std::string>> applications;
	std::vector<std::string> applicationVersions;
	std::vector<sf::RectangleShape> buttons;
	sf::Font font;

	void scanApplications() {
		fs::path root = "";
		fs::path binDir = root / "bin";

		for (const auto& versionDir : fs::directory_iterator(binDir)) {
			if (!fs::is_directory(versionDir)) continue;

			// Scan for any file with a .metadata extension within the version directory
			for (const auto& entry : fs::directory_iterator(versionDir)) {
				if (entry.is_regular_file() && entry.path().extension() == ".metadata") {
					std::ifstream metadata(entry.path());
					std::string line;
					std::string versionInfo;

					// Read version information
					std::getline(metadata, versionInfo);

					for (const auto& projectDir : fs::directory_iterator(versionDir)) {
						if (!fs::is_directory(projectDir)) continue;

						// Find the metadata file with the same name as the project folder
						fs::path projectMetadataFile = projectDir / projectDir.path().filename().replace_extension(".metadata");
						if (fs::exists(projectMetadataFile)) {
							std::ifstream projectMetadata(projectMetadataFile);
							std::string projectLine;
							std::string executablePath;
							std::string projVersion;

							while (std::getline(projectMetadata, projectLine)) {
								if (projectLine.find("ExecutablePath:") != std::string::npos) {
									executablePath = projectLine.substr(projectLine.find(":") + 2);
								}

								if (projectLine.find("Version:") != std::string::npos){
									projVersion = projectLine.substr(projectLine.find(":") + 2);
									break;
								}
							}
							applications.emplace_back(executablePath, projectDir.path().string());
							applicationVersions.emplace_back(projVersion);
						}
					}
				}
			}
		}
	}

	void createUI() {
		if (!font.loadFromFile("arial.ttf")) { // Load font
			std::cerr << "Failed to load font" << std::endl;
			return;
		}

		// Create buttons for each application
		for (size_t i = 0; i < applications.size(); ++i) {
			sf::RectangleShape button(sf::Vector2f(300, 60)); // Set button size
			button.setFillColor(sf::Color(100, 100, 100)); // Set button color
			button.setOutlineThickness(2); // Set outline thickness
			button.setOutlineColor(sf::Color::White); // Set outline color
			button.setPosition(50, 100 + 80 * i); // Set button position

			// Extract filename without extension
			std::string buttonText = applications[i].first;
			size_t dotPos = buttonText.find_last_of(".");
			if (dotPos != std::string::npos) {
				buttonText = buttonText.substr(0, dotPos);
			}

			// Append version information
			buttonText += " - Version: " + applicationVersions[i];

			sf::Text buttonTextObj;
			buttonTextObj.setFont(font);
			buttonTextObj.setString(buttonText); // Set button text
			buttonTextObj.setCharacterSize(17); // Set text size
			buttonTextObj.setFillColor(sf::Color::White); // Set text color

			// Center text within the button
			sf::FloatRect textBounds = buttonTextObj.getLocalBounds();
			buttonTextObj.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
			buttonTextObj.setPosition(button.getPosition().x + button.getSize().x / 2.0f, button.getPosition().y + button.getSize().y / 2.0f);

			buttons.push_back(button);
			uiTexts.push_back(buttonTextObj);
		}
	}

	void drawUI() {
		for (const auto& button : buttons) {
			window.draw(button);
		}
		for (const auto& text : uiTexts) {
			window.draw(text);
		}
	}

	void handleMouseClick(const sf::Event::MouseButtonEvent& mouse) {
		for (size_t i = 0; i < buttons.size(); ++i) {
			if (buttons[i].getGlobalBounds().contains(mouse.x, mouse.y)) {
				launchApplication(applications[i].first, applications[i].second);
				break;
			}
		}
	}

	void launchApplication(const std::string& executablePath, const std::string& projectPath) {
		// Construct the command to change directory and launch the application
		std::string command = "cd /D \"" + projectPath + "\" && start \"\" \"" + executablePath + "\"";

		//std::cout << "Launching command: " << command << std::endl; // Print the command for debugging

		// Execute the command
		int result = std::system(command.c_str());

		if (result == -1) {
			std::cerr << "Error launching application: " << strerror(errno) << std::endl;
		}
	}

	std::vector<sf::Text> uiTexts;
};

int main() {
	Launcher launcher;
	launcher.run();

	return 0;
}
