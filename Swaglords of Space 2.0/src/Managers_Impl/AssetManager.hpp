#pragma once

#include <map>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <fstream>

namespace _Swag {
	namespace _Asset::_Crypting {

		static sf::Image DatToImg(const char* datFilePath);
		static sf::SoundBuffer DatToAudio(const char* datFilePath);
		static std::string DatToString(const char* datfilePath);
	}

	class _AssetManager
	{
	public:
		_AssetManager() { }
		~_AssetManager() { }
		void ShutDown();

		void LoadTexture(std::string name, std::string fileName, bool asEncryptyed);
		sf::Texture& GetTexture(std::string name);

		void LoadFont(std::string name, std::string fileName);
		sf::Font& GetFont(std::string name);

		void LoadSoundBuffer(std::string name, std::string fileName, bool asEncrypted);
		sf::SoundBuffer& GetSoundBuffer(std::string name);

		void LoadSound(std::string name, sf::SoundBuffer& soundbuffer);
		sf::Sound& GetSound(std::string name);

		void LoadMusic(std::string name, std::string fileName);
		sf::Music& GetMusic(std::string name);

	private:
		std::map<std::string, sf::Texture> _textures;
		std::vector<std::string> _textureNames;
		std::map<std::string, sf::Image> _images;

		std::map<std::string, sf::Font> _fonts;
		std::map<std::string, sf::SoundBuffer> _soundBuffer;
		std::map<std::string, sf::Sound> _sound;
		std::map<std::string, sf::Music> _music;

	private:
		bool isShutDown = false;
	};
}