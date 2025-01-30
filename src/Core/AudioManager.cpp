#include <Core/AudioManager.h>

#include <iostream>
#include <unordered_map>
#include <SFML/Audio.hpp>

AudioManager* AudioManager::s_instance{ nullptr };

AudioManager* AudioManager::getInstance()
{
	if (s_instance == nullptr)
	{
		s_instance = new AudioManager();
	}
	return s_instance;
}

// ######################		SoundEffects		######################
bool AudioManager::loadSound(const std::string& name, const std::string& filepath)
{
	auto buffer = std::make_shared<sf::SoundBuffer>();
	if (!buffer->loadFromFile(filepath)) {
		std::cerr << "Error: Couldn't load sound form: " << filepath << std::endl;
		return false;
	}

	m_soundBuffers[name] = buffer;
	return true;
}

void AudioManager::playSound(const std::string& name) {
    // Search for the buffer of the corresponding sound
    auto soundBufferIt = m_soundBuffers.find(name);
    if (soundBufferIt != m_soundBuffers.end()) {
        // Create or re-use an instance of sf::Sound
        auto& sound = m_activeSounds[name];
        if (!sound) {
            sound = std::make_shared<sf::Sound>();
        }
        sound->setBuffer(*soundBufferIt->second);
        sound->setVolume(soundVolume);
        sound->play();
    }
    else {
        std::cerr << "Error: The sound '" << name << "' is not loaded." << std::endl;
    }
}

// ######################		Music		######################
bool AudioManager::loadMusic(const std::string& name, const std::string& filepath) {
    auto music = std::make_shared<sf::Music>();
    if (!music->openFromFile(filepath)) {
        std::cerr << "Error: Couldn't load music form: " << filepath << std::endl;
        return false;
    }
    m_music[name] = music;
    return true;
}

void AudioManager::playMusic(const std::string& name) {
    auto musicIt = m_music.find(name);
    if (musicIt != m_music.end()) {
        musicIt->second->play();
    }
    else {
        std::cerr << "Error: The music '" << name << "' is not loaded." << std::endl;
    }
}