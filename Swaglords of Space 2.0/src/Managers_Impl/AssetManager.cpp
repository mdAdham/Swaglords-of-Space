#include "AssetManager.hpp"
#include "Core/Log.hpp"

namespace _Swag {

	sf::Image _Asset::_Crypting::DatToImg(const char* datFilePath)
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

			_SWAG_TRACE("'{0}' has been successfully loaded!", datFilePath);
			return image;
		}
		else
		{
			_SWAG_ERROR("Failed to open the asset file: '{0}'", datFilePath);
			return sf::Image();
		}
	}

	sf::SoundBuffer _Asset::_Crypting::DatToAudio(const char* datFilePath)
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

			_SWAG_TRACE("'{0}' has been successfully loaded!", datFilePath);

			return soundBuffer;
		}
		else
		{
			_SWAG_ERROR("Failed to open the asset file: '{0}'", datFilePath);

			return sf::SoundBuffer();
		}
	}
	
	//Do this in future!!!!!!
    std::string _Asset::_Crypting::DatToString(const char* datfilePath)
    {
		std::fstream file(datfilePath);

        return std::string();
    }

	void _AssetManager::ShutDown()
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
	void _AssetManager::LoadTexture(std::string name, std::string fileName, bool asEncryptyed)
	{
		for (auto& i : _textures)
			if (i.first == name)
				return;

		if (asEncryptyed)
		{
			sf::Image img = _Asset::_Crypting::DatToImg(fileName.c_str());
			sf::Texture tex = {};
			if (tex.loadFromImage(img))
			{
				this->_textures[name] = tex;
				this->_images[name] = img;
				_SWAG_TRACE("{0}:'{1}' has been loaded!", name, fileName);
			}
		}
		else
		{
			sf::Texture tex;
			if (tex.loadFromFile(fileName))
			{
				this->_textures[name] = tex;
				_images[name] = this->_textures[name].copyToImage();
				_SWAG_TRACE("{0}:'{1}' has been loaded!", name, fileName);
			}
			else
			{
				_SWAG_ERROR("Failed to load the asset- {0}: '{1}'", name, fileName);
			}
		}
	}

	sf::Texture& _AssetManager::GetTexture(std::string name)
	{
		for (auto& i : _textures)
		{
			if (i.first == name)
				return this->_textures.at(name);
		}
		_SWAG_ERROR("Texture is not found! - {0}", name);
	}

	void _AssetManager::LoadFont(std::string name, std::string fileName)
	{
		for (auto& i : _fonts)
			if (i.first == name)
				return;

		sf::Font font;
		if (font.loadFromFile(fileName))
		{
			this->_fonts[name] = font;
			_SWAG_TRACE("Font successfully loaded! - {0}: '{1}'", name, fileName);
		}
		else
		{
			_SWAG_ERROR("Failed to load the Font {0}: '{1}'", name, fileName);
		}
	}

	sf::Font& _AssetManager::GetFont(std::string name)
	{
		return this->_fonts.at(name);
	}

	void _AssetManager::LoadSoundBuffer(std::string name, std::string fileName, bool asEncrypted)
	{
		for(auto& i : _soundBuffer)
			if (i.first == name)
				return;

		if (asEncrypted)
		{
			sf::SoundBuffer soundbuffer = _Asset::_Crypting::DatToAudio(fileName.c_str());
			this->_soundBuffer[name] = soundbuffer;
			_SWAG_TRACE("Sucessfully loaded the Sound! {0}: '{1}'", name, fileName);
		}
		else
		{
			sf::SoundBuffer soundbuffer;
			if (soundbuffer.loadFromFile(fileName))
			{
				this->_soundBuffer[name] = soundbuffer;
				_SWAG_TRACE("Sucessfully loaded the Sound! {0}: '{1}'", name, fileName);
			}
			else
			{
				_SWAG_ERROR("Failed to load the asset {0}: {1}", name, fileName);
			}
		}
	}

	sf::SoundBuffer& _AssetManager::GetSoundBuffer(std::string name)
	{
		return this->_soundBuffer[name];
	}

	void _AssetManager::LoadSound(std::string name, sf::SoundBuffer& soundBuffer)
	{
		for (auto& i : _sound)
			if (i.first == name)
				return;

		this->_sound[name].setBuffer(soundBuffer);
	}

	sf::Sound& _AssetManager::GetSound(std::string name)
	{
		return this->_sound[name];
	}

	void _AssetManager::LoadMusic(std::string name, std::string fileName)
	{
		if (!this->_music[name].openFromFile(fileName))
		{
			_SWAG_ERROR("Failed to load the asset {0}: {1}", name, fileName);
		}
		else _SWAG_TRACE("Sucessfully loaded the asset! {0}: '{1}'", name, fileName);
	}

	sf::Music& _AssetManager::GetMusic(std::string name)
	{
		return this->_music[name];
	}

}