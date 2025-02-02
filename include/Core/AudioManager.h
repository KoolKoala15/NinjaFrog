#pragma once

#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>
#include <memory>


class AudioManager {
public:
    static AudioManager* getInstance();

    ~AudioManager();

    // Sound Effects
    bool loadSound(const std::string& name, const std::string& filepath);
    void playSound(const std::string& name);

    // Music
    bool loadMusic(const std::string& name, const std::string& filepath);
    void playMusic(const std::string& name);

private:

    AudioManager() = default;

    static AudioManager* s_instance;

    float soundVolume = 100.0f;

    // Sfx
    std::unordered_map<std::string, std::unique_ptr<sf::SoundBuffer>> m_soundBuffers;
    std::unordered_map<std::string, std::unique_ptr<sf::Sound>> m_activeSounds;
    
    // Music
    std::unordered_map<std::string, std::unique_ptr<sf::Music>> m_music;
};