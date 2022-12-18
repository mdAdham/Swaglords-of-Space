#pragma once

#include <map>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Core/log.h"

namespace Swag
{
	#define asserts this->_data->assets
	class AssetManager
	{
	public:
		AssetManager() { }
		~AssetManager() { }
		void ShutDown();

		void LoadTexture(std::string name, std::string fileName);
		sf::Texture& GetTexture(std::string name);

		void LoadFont(std::string name, std::string fileName);
		sf::Font& GetFont(std::string name);

		void LoadSoundBuffer(std::string name, std::string fileName);
		sf::SoundBuffer& GetSoundBuffer(std::string name);

		void LoadSound(std::string name, sf::SoundBuffer& soundbuffer);
		sf::Sound& GetSound(std::string name);

		void LoadMusic(std::string name, std::string fileName);
		sf::Music& GetMusic(std::string name);

	private:
		std::map<std::string, sf::Texture> _textures;
		std::map<std::string, sf::Font> _fonts;
		std::map<std::string, sf::SoundBuffer> _soundBuffer;
		std::map<std::string, sf::Sound> _sound;
		std::map<std::string, sf::Music> _music;
	};
}
