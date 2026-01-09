//
// Created by jay on 09/01/2026.
//

#include <filesystem>
#include "TextureManager.h"

#include <iostream>

std::map<std::string,sf::Texture> TextureManager::textures = {};

void TextureManager::initialiseTextures(const std::string& path){
    const std::filesystem::path directory{path};

    for (auto const& dir_entry : std::filesystem::directory_iterator{directory}){
        auto texturePath = dir_entry.path().string();

        // Only accept .png
        if (dir_entry.path().extension().string() == ".png"){
            TextureManager::textures.emplace(texturePath, sf::Texture(texturePath));
            std::cout << "Initialised texture: " << (dir_entry) << std::endl;
        }
        else{
            std::cout << "Rejected file: " << (dir_entry) << std::endl;
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
