#pragma once

#include <SFML/Audio.hpp>

#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <memory>
#include <random>

#include "Core/Core.hpp"

namespace _Swag {

	class _AssetManager;

	class AudioManager
	{
	public:
		explicit AudioManager(_AssetManager& asset, size_t soundPoolsSize = 32);
		//void Init(_AssetManager& asset, size_t soundPoolsSize = 32);

		~AudioManager();
		//AudioManager(const AudioManager&) = delete;
		//AudioManager& operator=(const AudioManager&) = delete;

		void update(float dt);

		void setListnerPosition(sf::Vector2f position, sf::Vector3f up = sf::Vector3f(0.f, 0.f, 1.f), sf::Vector3f direction = sf::Vector3f(0.f, 0.f, 0.f));
		sf::Vector2f getListnerPosition() const;

		void playSFX(std::string_view soundId, sf::Vector2f position);
		void playSFX(std::string_view soundId);

		void playRandomSFX(std::string_view GroupId, sf::Vector2f position);

		void playLooping(std::string_view soundID, uint32_t objectID, sf::Vector2f position);
		void updateLooping(uint32_t objectId, sf::Vector2f position);
		void stopLooping(uint32_t objectId, float fadeTime = 1.0f);

		void playMusic(std::string_view musicId, float fadeTime = 0.0f);
		void stopMusic(float fadeTime = 1.0f);

		void setSFXVolume(float volume);
		void setMusicVolume(float volume);

		float getSFXVolume() const;
		float getMusicVolume() const;

	private:
		struct SoundInstance
		{
			Scope<sf::Sound> sound;
			bool looping = false;
			uint32_t objectId = 0;

			float targetVolume = 0.f;
			float currentVolume = 0.f;
			float fadeSpeed = 0.f;
			bool fadingOut = false;
		};

		struct MusicState
		{
			sf::Music* music = nullptr;
			float targetVolume = 0.f;
			float currentVolume = 0.f;
			float fadeSpeed = 0.f;
			bool fadingOut = false;
		};

		

	private:
		_AssetManager& m_assets;

		std::vector<SoundInstance> m_soundPool;

		std::unordered_map<uint32_t, size_t> m_loopingSounds;

		Scope<MusicState> m_currentMusic;
		Scope<MusicState> m_nextMusic;

		float m_sfxVolume = 100.f;
		float m_musicVolume = 100.f;

		sf::Vector2f m_listnerPosition{};

		std::mt19937 rng;

	private:
		size_t findFreeSound();

		size_t findSoundForObject(uint32_t objectId);

		void updateSounds();

		void updateLooping(float dt);

		void updateMusic(float dt);

		void setMusicVolume(MusicState* state, float volume);
	};
}