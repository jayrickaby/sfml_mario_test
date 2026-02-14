//
// Created by jay on 09/02/2026.
//

#include "SoundManager.h"

#include <iostream>
#include <SFML/Audio/SoundSource.hpp>

#include "GameManager.h"
#include "ManagerUtilities.h"


std::map<std::string, sf::SoundBuffer> SoundManager::bufferedSounds;
std::vector<std::unique_ptr<sf::Sound>> SoundManager::sounds;
std::string SoundManager::fullPath;

bool SoundManager::isSoundFile(const std::string& name){
    if (!isInitialised()){
        throw std::runtime_error("No sounds initialised");
    }
    return bufferedSounds.contains(name);
}

void SoundManager::clearStoppedSounds(){
    std::erase_if(sounds,
        [](const auto& sound) {
            return sound->getStatus() == sf::Sound::Status::Stopped;
        }
    );

}

void SoundManager::initialiseSoundFiles(){
    fullPath = GameManager::getAssetPath() + "sounds/";

    const std::vector files(ManagerUtilities::findFiles(fullPath, {".ogg"}));

    for (const auto& file : files){
        bufferedSounds.emplace(file, sf::SoundBuffer(fullPath + file));
        std::cout << "Initialised sound: " << file << std::endl;
    }
}

bool SoundManager::isInitialised(){
    return ManagerUtilities::isInitialised(bufferedSounds);
}

void SoundManager::playSoundFile(const std::string& name){
    if (!isSoundFile(name)){
        throw std::runtime_error("Could not find sound: \"" + name + "\"");
    }
    std::cout << "Playing sound: \"" << name << "\"" << std::endl;
    auto sound = std::make_unique<sf::Sound>(bufferedSounds.at(name));
    sound->play();
    sounds.emplace_back(std::move(sound));
}
