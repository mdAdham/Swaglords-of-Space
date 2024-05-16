#include "AssetManager.h"
#include <iostream>

namespace Asset {

	sf::Image DatToImg(const char* datFilePath)
	{
		std::fstream data_file(datFilePath, std::ios::in | std::ios::binary);
		if (data_file.is_open())
		{
			sf::Vector2u imageSize;
			data_file.read(reinterpret_cast<char*>(&imageSize.x), sizeof(imageSize.x));
			data_file.read(reinterpret_cast<char*>(&imageSize.y), sizeof(imageSize.y));

			std::vector<sf::Uint8> pixelData(imageSize.x * imageSize.y * 4);

			data_file.read(reinterpret_cast<char*>(pixelData.data()), pixelData.size());

			sf::Image image;
			image.create(imageSize.x, imageSize.y, pixelData.data());
			data_file.close();
			return image;
		}
		return sf::Image();
	}

	sf::SoundBuffer DatToAudio(const char* datFilePath)
	{
		std::fstream data_file(datFilePath, std::ios::in | std::ios::binary);
		if (data_file.is_open())
		{
			unsigned int sampleRate;
			unsigned int channelCount;
			data_file.read(reinterpret_cast<char*>(&sampleRate), sizeof(sampleRate));
			data_file.read(reinterpret_cast<char*>(&channelCount), sizeof(channelCount));

			data_file.seekg(0, std::ios::end);
			std::streampos length = data_file.tellg() - (std::streampos)sizeof(sampleRate) - sizeof(channelCount);
			data_file.seekg(sizeof(sampleRate) + sizeof(channelCount));

			std::vector<sf::Int16> audioData(length / sizeof(sf::Int16));
			data_file.read(reinterpret_cast<char*>(audioData.data()), length);

			sf::SoundBuffer soundBuffer;
			soundBuffer.loadFromSamples(audioData.data(), audioData.size(), channelCount, sampleRate);

			data_file.close();

			return soundBuffer;
		}

		return sf::SoundBuffer();
	}
}

namespace Swag
{
	void AssetManager::ShutDown()
	{
		if (!this->isShutDown)
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

			this->isShutDown = true;
		}
	}
	void AssetManager::LoadTexture(std::string name, std::string fileName, bool asEncryptyed)
	{
		if (asEncryptyed)
		{
			sf::Image img = Asset::DatToImg(fileName.c_str());
			sf::Texture tex = {};
			if (tex.loadFromImage(img))
			{
				this->_textures[name] = tex;
				_images[name] = img;
			}
		}
		else
		{
			sf::Texture tex;
			if (tex.loadFromFile(fileName))
			{
				this->_textures[name] = tex;
				_images[name] = this->_textures[name].copyToImage();
			}
			else
			{
				SWAG_ERROR("Cannot Load From File {0}: {1}", name, fileName);
			}
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

	void AssetManager::LoadSoundBuffer(std::string name, std::string fileName, bool asEncrypted)
	{
		if (asEncrypted)
		{
			sf::SoundBuffer soundbuffer = Asset::DatToAudio(fileName.c_str());
			this->_soundBuffer[name] = soundbuffer;
		}
		else
		{
			sf::SoundBuffer soundbuffer;
			if (soundbuffer.loadFromFile(fileName))
				this->_soundBuffer[name] = soundbuffer;
			else
			{
				SWAG_ERROR("Cannot Load From File {0}: {1}", name, fileName);
			}
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
	sf::Image& AssetManager::GetAsImage(std::string name)
	{
		return _images[name];
	}
}