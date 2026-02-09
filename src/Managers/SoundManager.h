//
// Created by jay on 09/02/2026.
//

#ifndef SFML_MARIO_TEST_SOUNDMANAGER_H
#define SFML_MARIO_TEST_SOUNDMANAGER_H
#include <map>
#include <string>
#include <SFML/Audio.hpp>


class SoundManager{
public:
    SoundManager() = default;
    ~SoundManager() = default;

    static void clearStoppedSounds();

    static void initialiseSoundFiles();
    static void playSoundFile(const std::string& name);
    static bool isSoundFile(const std::string& name);
    static bool isInitialised();

private:
    static std::map<std::string, sf::SoundBuffer> bufferedSounds;
    static std::vector<sf::Sound> sounds;
    static std::string fullPath;
};


#endif //SFML_MARIO_TEST_SOUNDMANAGER_H