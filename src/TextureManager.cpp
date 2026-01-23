//
// Created by jay on 09/01/2026.
//

#include <filesystem>
#include "TextureManager.h"

#include <iostream>

std::map<std::string,sf::Texture> TextureManager::textures = {};

void TextureManager::initialiseTextures(const std::string& path){
    if (!std::filesystem::is_directory(path)){
        throw std::runtime_error("Could not find directory: \"" + path + "\"");
    }
    const std::filesystem::path directory{path};

    for (auto const& dirEntry : std::filesystem::directory_iterator{directory}){
        auto texturePath = dirEntry.path().string();

        // Only accept .png
        if (dirEntry.path().extension().string() == ".png"){
            TextureManager::textures.emplace(texturePath, sf::Texture(texturePath));
            std::cout << "Initialised texture: " << (dirEntry) << std::endl;
        }
        else{
            std::cout << "Incompatible texture file: " << (dirEntry) << std::endl;
        }
    }
}

sf::Texture& TextureManager::loadTexture(const std::string& path){
    if (TextureManager::textures.size() == 0){
        throw std::runtime_error("No textures initialised!");
    }
    else if (!TextureManager::textures.contains(path)){
        throw std::runtime_error("Could not find texture: \"" + path + "\"");
    }
    else {
        std::cout << "Loaded texture: \"" << path << "\"" << std::endl;
        return TextureManager::textures.at(path);
    }
}
