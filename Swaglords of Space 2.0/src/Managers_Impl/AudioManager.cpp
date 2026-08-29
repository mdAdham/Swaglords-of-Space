#include "AudioManager.hpp"

#include "AssetManager.hpp"

#include <algorithm>
#include <chrono>
#include <random>
#include <stdexcept>

namespace _Swag {
	AudioManager::AudioManager(_AssetManager& asset, size_t soundPoolsSize)
		:m_assets(asset)
	{
		std::random_device rd;
		rng.seed(rd());

		m_soundPool.reserve(soundPoolsSize);

		for (size_t i = 0; i < soundPoolsSize; i++)
		{
			SoundInstance sound;
			sound.sound = nullptr;
			m_soundPool.push_back(std::move(sound));
		}
	}
	AudioManager::~AudioManager()
	{
		for (auto& i : m_soundPool)
		{
			if (i.sound)
				i.sound->stop();
		}
	}
	//void AudioManager::Init(_AssetManager& asset, size_t soundPoolsSize)
	//{

	//}
	void AudioManager::update(float dt)
	{
		updateSounds();
		updateLooping(dt);
		updateMusic(dt);
	}
	void AudioManager::setListnerPosition(sf::Vector2f position, sf::Vector3f up, sf::Vector3f direction)
	{
		m_listnerPosition = position;

		sf::Listener::setPosition(position.x, position.y, 0.f);
		sf::Listener::setUpVector(up.x, up.y, up.z);
		sf::Listener::setDirection(direction.x, direction.y, direction.z);
	}
	sf::Vector2f AudioManager::getListnerPosition() const
	{
		return m_listnerPosition;
	}
	void AudioManager::playSFX(std::string_view soundId, sf::Vector2f position)
	{
		const size_t index = findFreeSound();

		auto& instance = m_soundPool[index];

		instance.looping = false;

		instance.sound = CreateScope<sf::Sound>(m_assets.GetSound(soundId.data()));

		instance.sound->setPosition(position.x, position.y, 0.f);

		instance.sound->setRelativeToListener(false);

		instance.sound->setVolume(m_sfxVolume);

		instance.sound->play();
	}
	void AudioManager::playSFX(std::string_view soundId)
	{
		const size_t index = findFreeSound();

		auto& instance = m_soundPool[index];

		instance.looping = false;

		instance.sound = CreateScope<sf::Sound>(m_assets.GetSound(soundId.data()));

		instance.sound->setRelativeToListener(true);

		instance.sound->setVolume(m_sfxVolume);

		instance.sound->play();
	}
	void AudioManager::playRandomSFX(std::string_view GroupId, sf::Vector2f position)
	{
		const auto sounds = m_assets.getSoundGroup(GroupId);
		//_SWAG_DEBUGS("{0} - {1}", GroupId, sounds.size());

		if (sounds.empty())
			return;

		std::uniform_int_distribution<size_t> dist(0, sounds.size() - 1);

		playSFX(sounds[dist(rng)], position);
	}
	void AudioManager::playLooping(std::string_view soundID, uint32_t objectID, sf::Vector2f position)
	{
		if (m_loopingSounds.find(objectID) != m_loopingSounds.end())
		{
			updateLooping(objectID, position);
			return;
		}

		const size_t index = findFreeSound();

		auto& instance = m_soundPool[index];

		instance.sound = CreateScope<sf::Sound>(m_assets.GetSound(soundID.data()));

		instance.looping = true;
		instance.fadeSpeed = instance.currentVolume / 0.5f;

		instance.objectId = objectID;

		instance.sound->setLoop(true);

		instance.sound->setPosition(position.x, position.y, 0.f);

		instance.sound->setVolume(m_sfxVolume);

		instance.sound->play();

		m_loopingSounds[objectID] = index;
	}
	void AudioManager::updateLooping(uint32_t objectId, sf::Vector2f position)
	{
		auto it = m_loopingSounds.find(objectId);
		if (it == m_loopingSounds.end())
			return;

		auto& instance = m_soundPool[it->second];

		if (!instance.sound)
			return;

		instance.sound->setPosition(position.x, position.y, 0.f);
	}
	void AudioManager::stopLooping(uint32_t objectId, float fadeTime)
	{
		auto it = m_loopingSounds.find(objectId);
		if (it == m_loopingSounds.end())
			return;

		auto& instance = m_soundPool[it->second];

		instance.currentVolume -= instance.fadeSpeed * fadeTime;

		instance.currentVolume = std::min(0.f, instance.currentVolume);

		instance.sound->setVolume(instance.currentVolume);

		if (instance.currentVolume <= 0.f)
		{
			instance.currentVolume = 0.f;
			instance.fadingOut = true;
			if (instance.sound)
				instance.sound->stop();

			instance.sound.reset();
			instance.looping = false;

			m_loopingSounds.erase(it);
		}
	}
	void AudioManager::playMusic(std::string_view musicId, float fadeTime)
	{
		auto state = CreateScope<MusicState>();

		state->music = &m_assets.GetMusic(musicId.data());

		state->currentVolume = 0.f;
		state->targetVolume = m_musicVolume;
		state->fadingOut = false;

		if (fadeTime > 0.f)
			state->fadeSpeed = getMusicVolume() / fadeTime;
		else
		{
			state->fadeSpeed = 0.f;
			state->currentVolume = m_musicVolume;
		}

		state->music->setVolume(state->currentVolume);

		state->music->setLoop(true);

		state->music->play();

		m_nextMusic = std::move(state);
	}
	void AudioManager::stopMusic(float fadeTime)
	{
		if (!m_currentMusic)
			return;

		if (fadeTime <= 0.f)
		{
			m_currentMusic->music->stop();
			m_currentMusic.reset();
			return;
		}

		m_currentMusic->fadingOut = true;
		m_currentMusic->targetVolume = 0.f;

		m_currentMusic->fadeSpeed = m_currentMusic->currentVolume / fadeTime;
	}
	void AudioManager::setSFXVolume(float volume)
	{
		m_sfxVolume = std::clamp(volume, 0.f, 100.f);

		for (auto& instance : m_soundPool)
		{
			if (instance.sound)
				instance.sound->setVolume(m_sfxVolume);
		}
	}
	void AudioManager::setMusicVolume(float volume)
	{
		m_musicVolume = std::clamp(volume, 0.f, 100.f);

		if (m_currentMusic)
			setMusicVolume(m_currentMusic.get(), m_currentMusic->currentVolume);
	}
	float AudioManager::getSFXVolume() const
	{
		return m_sfxVolume;
	}
	float AudioManager::getMusicVolume() const
	{
		return m_musicVolume;
	}
	size_t AudioManager::findFreeSound()
	{
		for (size_t i = 0; i < m_soundPool.size(); i++)
		{
			if (!m_soundPool[i].sound)
				return i;

			if (m_soundPool[i].sound->getStatus() != sf::SoundSource::Status::Playing)
				return i;
		}

		for (size_t i = 0; i < m_soundPool.size(); i++)
		{
			if (!m_soundPool[i].looping)
				return i;
		}

		return 0;
	}
	size_t AudioManager::findSoundForObject(uint32_t objectId)
	{
		return size_t();
	}
	void AudioManager::updateSounds()
	{
		for (auto& instance : m_soundPool)
		{
			if (!instance.sound)
				continue;

			if (!instance.looping && instance.sound->getStatus() != sf::SoundSource::Status::Playing)
				instance.sound.reset();
		}
	}
	void AudioManager::updateLooping(float dt)
	{

	}
	void AudioManager::updateMusic(float dt)
	{
		if (m_currentMusic && m_nextMusic)
		{
			m_currentMusic->currentVolume -= m_currentMusic->fadeSpeed * dt;
			m_currentMusic->currentVolume = std::max(0.f, m_currentMusic->currentVolume);

			m_currentMusic->music->setVolume(m_currentMusic->currentVolume);

			if (m_currentMusic->currentVolume <= 0.f)
			{
				m_currentMusic->music->stop();
				m_currentMusic.reset();
			}
		}

		if (m_nextMusic)
		{
			m_nextMusic->currentVolume += m_nextMusic->fadeSpeed * dt;
			m_nextMusic->currentVolume = std::min(m_nextMusic->targetVolume, m_nextMusic->currentVolume);

			m_nextMusic->music->setVolume(m_nextMusic->currentVolume);

			if (m_nextMusic->currentVolume >= m_nextMusic->targetVolume)
				m_currentMusic = std::move(m_nextMusic);
		}
	}
	void AudioManager::setMusicVolume(MusicState* state, float volume)
	{
	}
}