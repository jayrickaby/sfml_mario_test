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

#include "GameManager.h"

std::map<std::string,sf::Texture> TextureManager::textures = {};
std::string TextureManager::fullPath = "";

// @TODO Texture atlassing!
void TextureManager::initialiseTextures(){
    fullPath = GameManager::getAssetPath() + "textures/";
    if (!std::filesystem::is_directory(fullPath)){
        throw std::runtime_error("Could not find directory: \"" + fullPath + "\"");
    }
    const std::filesystem::path directory{fullPath};

    for (auto const& dirEntry : std::filesystem::recursive_directory_iterator{directory}){
        if (dirEntry.is_directory()){
            continue;
        }

        std::string texturePath = std::filesystem::relative(dirEntry, fullPath).string();
        // Only accept .png
        if (dirEntry.path().extension().string() == ".png"){
            textures.emplace(texturePath, sf::Texture(fullPath + texturePath));
            std::cout << "Initialised texture: " << (texturePath) << std::endl;
        }
        else{
            std::cout << "Incompatible texture file: " << (texturePath) << std::endl;
        }
    }
}

bool TextureManager::isTexture(const std::string& name){
    if (!isInitialised()){
        throw std::runtime_error("No textures initialised!");
    }
    if (!textures.contains(name)){
        return false;
    }
    return true;
}

sf::Texture& TextureManager::loadTexture(const std::string& name){
    if (!isTexture(name)){
        throw std::runtime_error("Could not find texture: \"" + name + "\"");
    }
    std::cout << "Loaded texture: \"" << name << "\"" << std::endl;
    return textures.at(name);
}

bool TextureManager::isInitialised(){
    if (textures.empty()){
        return false;
    }
    return true;
}
