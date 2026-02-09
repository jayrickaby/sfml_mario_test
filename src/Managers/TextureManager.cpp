//
// Created by jay on 09/01/2026.
//

#include <filesystem>
#include "TextureManager.h"
#include <iostream>

#include "GameManager.h"
#include "ManagerUtilities.h"

std::map<std::string,sf::Texture> TextureManager::textures = {};
std::string TextureManager::fullPath = "";

// @TODO Texture atlassing!
void TextureManager::initialiseTextures(){
    fullPath = GameManager::getAssetPath() + "textures/";

    const std::vector files(ManagerUtilities::findFiles(fullPath, {".png"}));

    for (const auto& file : files){
        textures.emplace(file, sf::Texture(fullPath + file));
        std::cout << "Initialised texture: " << (file) << std::endl;
    }
}

bool TextureManager::isTexture(const std::string& name){
    if (!isInitialised()){
        throw std::runtime_error("No textures initialised!");
    }
    return textures.contains(name);
}

bool TextureManager::isInitialised(){
    return ManagerUtilities::isInitialised(textures);
}

sf::Texture& TextureManager::loadTexture(const std::string& name){
    if (!isTexture(name)){
        throw std::runtime_error("Could not find texture: \"" + name + "\"");
    }
    std::cout << "Loaded texture: \"" << name << "\"" << std::endl;
    return textures.at(name);
}