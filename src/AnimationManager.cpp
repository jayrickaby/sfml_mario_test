//
// Created by jay on 23/01/2026.
//

#include "AnimationManager.h"

#include <fstream>
#include <iostream>

#include "Animation.h"
#include "TextureManager.h"

std::map<std::string, std::map<std::string, Animation>> AnimationManager::animations = {};

void AnimationManager::initialiseAnimations(const std::string& path){
    if (!std::filesystem::is_directory(path)){
        throw std::runtime_error("Could not find directory: \"" + path + "\"");
    }
    const std::filesystem::path animationFilePath(path);

    for (auto const dirEntry : std::filesystem::directory_iterator(animationFilePath)){
        auto animationPath = dirEntry.path().string();

        if (dirEntry.path().extension() == ".json"){
            std::ifstream animationFileData(animationPath);
            nlohmann::json file = nlohmann::json::parse(animationFileData);
            std::map<std::string, Animation> animationsList;
            for (const auto& animationData : file["animations"]){
                Animation animation;
                for (const auto& frameData : animationData["frames"]){
                    Frame frame;
                    frame.rect = sf::IntRect({frameData["pos"][0],frameData["pos"][1]}, {frameData["size"][0], frameData["size"][1]});
                    animation.addFrame(frame);
                }
                animationsList.emplace(animationData["name"], animation);
            }
            AnimationManager::animations.emplace(animationPath, animationsList);
            std::cout << "Initialised animation file: " << (dirEntry) << std::endl;
        }
        else{
            std::cout << "Incompatible animation file: " << (dirEntry) << std::endl;
        }
    }
}

// @TODO return by reference instead of copying
std::map<std::string, Animation> AnimationManager::loadAnimation(const std::string& name){
    if (AnimationManager::animations.size() == 0){
        throw std::runtime_error("No animations initialised!");
    }
    else if (!AnimationManager::animations.contains(name)){
        throw std::runtime_error("Could not find animation: \"" + name + "\"");
    }
    else{
        std::cout << "Loaded animation: \"" << name << "\"" << std::endl;
        return AnimationManager::animations[name];
    }
}
