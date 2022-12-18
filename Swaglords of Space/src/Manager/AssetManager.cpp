#include "AssetManager.h"
#include <iostream>

namespace Swag
{
	void AssetManager::ShutDown()
	{
		for (auto& sound : this->_sound)
			this->_sound[sound.first].stop();
		
		for (auto& music : this->_music)
			this->_music[music.first].stop();

		for (auto& soundBuffer : this->_soundBuffer)
			this->_soundBuffer[soundBuffer.first].~SoundBuffer();

		for (auto& sound : this->_sound)
			this->_sound[sound.first].~Sound();

		for (auto& music : this->_music)
			this->_music[music.first].~Music();
	}
	void AssetManager::LoadTexture(std::string name, std::string fileName)
	{
		sf::Texture tex;
		if (tex.loadFromFile(fileName))
		{
			this->_textures[name] = tex;
		}
		else
		{
			SWAG_ERROR("Cannot Load From File {0}: {1}", name, fileName);
		}
	}

	sf::Texture& AssetManager::GetTexture(std::string name)
	{
		return this->_textures.at(name);
	}

	void AssetManager::LoadFont(std::string name, std::string fileName)
	{
		sf::Font font;
		if (font.loadFromFile(fileName))
		{
			this->_fonts[name] = font;
		}
		else
		{
			SWAG_ERROR("Cannot Load From File {0}: {1}", name, fileName);
		}
	}

	sf::Font& AssetManager::GetFont(std::string name)
	{
		return this->_fonts.at(name);
	}

	void AssetManager::LoadSoundBuffer(std::string name, std::string fileName)
	{
		sf::SoundBuffer soundbuffer;
		if (soundbuffer.loadFromFile(fileName))
			this->_soundBuffer[name] = soundbuffer;
		else
		{
			SWAG_ERROR("Cannot Load From File {0}: {1}", name, fileName);
		}
	}

	sf::SoundBuffer& AssetManager::GetSoundBuffer(std::string name)
	{
		return this->_soundBuffer[name];
	}

	void AssetManager::LoadSound(std::string name, sf::SoundBuffer& soundBuffer)
	{
		this->_sound[name].setBuffer(soundBuffer);
	}

	sf::Sound& AssetManager::GetSound(std::string name)
	{
		return this->_sound[name];
	}

	void AssetManager::LoadMusic(std::string name, std::string fileName)
	{
		if (!this->_music[name].openFromFile(fileName))
			SWAG_ERROR("Cannot Open From File {0}: {1}", name, fileName);
	}

	sf::Music& AssetManager::GetMusic(std::string name)
	{
		return this->_music[name];
	}
}