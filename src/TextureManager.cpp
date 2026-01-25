//
// Created by jay on 09/01/2026.
//

#include <filesystem>
#include "TextureManager.h"

#define STB_RECT_PACK_IMPLEMENTATION
#include "stb_rect_pack.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

std::map<std::string,sf::Texture> TextureManager::textures = {};

void TextureManager::initialiseTextures(const std::string& path){
    if (!std::filesystem::is_directory(path)){
        throw std::runtime_error("Could not find directory: \"" + path + "\"");
    }
    const std::filesystem::path directory{path};

    for (auto const& dirEntry : std::filesystem::recursive_directory_iterator{directory}){
        if (dirEntry.is_directory()){
            continue;
        }

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

bool TextureManager::isTexture(const std::string& path){
    if (!isInitialised()){
        throw std::runtime_error("No textures initialised!");
    }
    if (!textures.contains(path)){
        return false;
    }
    return true;
}

sf::Texture& TextureManager::loadTexture(const std::string& path){
    if (!isTexture(path)){
        throw std::runtime_error("Could not find texture: \"" + path + "\"");
    }
    std::cout << "Loaded texture: \"" << path << "\"" << std::endl;
    return textures.at(path);
}

bool TextureManager::isInitialised(){
    if (textures.empty()){
        return false;
    }
    return true;
}
