//
// Created by jay on 23/01/2026.
//

#include "AnimationManager.h"

#include <fstream>
#include <iostream>

#include "Animation.h"
#include "GameManager.h"
#include "ManagerUtilities.h"

std::map<std::string, std::map<std::string, Animation>> AnimationManager::animations = {};
std::string AnimationManager::fullPath = "";

void AnimationManager::initialiseAnimations(){
    fullPath = GameManager::getAssetPath() + "animations/";

    std::vector files(ManagerUtilities::findFiles(fullPath, {".json"}));

    for (std::string& file : files){
        std::map animationsList{parseAnimations(fullPath + file)};
        animations.emplace(file, animationsList);
        std::cout << "Initialised animation: \"" << file << "\"" << std::endl;
    }
}

std::map<std::string, Animation>* AnimationManager::loadAnimation(const std::string& name){
    if (animations.empty()){
        throw std::runtime_error("No animations initialised!");
    }
     if (!animations.contains(name)){
        throw std::runtime_error("Could not find animation: \"" + name + "\"");
    }
    std::cout << "Loaded animation: \"" << name << "\"" << std::endl;
    return &animations[name];
}

Frame AnimationManager::parseFrame(const nlohmann::basic_json<>& frameData){
    Frame frame;

    if (frameData.empty()){
        throw std::runtime_error("Frame has no data defined!");
    }

    if (frameData["pos"].empty()){
        throw std::runtime_error("Frame has no position defined!");
    }
    const sf::Vector2i framePos({frameData["pos"][0], frameData["pos"][1]});

    if (frameData["size"].empty()){
        throw std::runtime_error("Frame has no size defined!");
    }
    const sf::Vector2i frameSize({frameData["size"][0], frameData["size"][1]});

    frame.rect = sf::IntRect(framePos, frameSize);
    return frame;

}

std::map<std::string, Animation> AnimationManager::parseAnimations(const std::string& path){
    std::ifstream animationFileData(path);
    nlohmann::json file = nlohmann::json::parse(animationFileData);
    std::map<std::string, Animation> animationsList;
    for (const auto& animationData : file["animations"]){
        Animation animation{parseAnimation(animationData)};
        animationsList.emplace(animationData["name"], animation);
    }
    return animationsList;
}

Animation AnimationManager::parseAnimation(const nlohmann::basic_json<>& animationData){
    Animation animation(animationData["name"]);
    for (const auto& frameData : animationData["frames"]){
        animation.addFrame(parseFrame(frameData));
    }

    return animation;
}
