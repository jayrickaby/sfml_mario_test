//
// Created by jay on 25/01/2026.
//

#include "TileManager.h"

#include <fstream>
#include <filesystem>
#include <iostream>
#include <nlohmann/json.hpp>
#include "TextureManager.h"

std::map<std::string, Tile> TileManager::tiles = {};

void TileManager::initialiseTiles(const std::string& path){
    if (!std::filesystem::is_directory(path)){
        throw std::runtime_error("Could not find directory: \"" + path + "\"");
    }
    if (!TextureManager::isInitialised()){
        throw std::runtime_error("Cannot initialise tiles before initialising textures!");
    }
    const std::filesystem::path directory{path};

    for (auto const& dirEntry : std::filesystem::recursive_directory_iterator{directory}){
        if (dirEntry.is_directory()){
            continue;
        }
        auto modelPath = dirEntry.path().string();

        if (dirEntry.path().extension() == ".json"){
            Tile tile;
            std::ifstream modelFileData(modelPath);
            nlohmann::json file = nlohmann::json::parse(modelFileData);
            if (file["textures"].empty()){
                throw std::runtime_error("Model \"" + modelPath + "\" has no textures!");
            }
            for (const auto& texture : file["textures"]){
                std::string textureName = "assets/textures/" + texture.get<std::string>();
                if (TextureManager::isTexture(textureName)){
                    tile.addTexture(TextureManager::loadTexture(textureName));
                }
            }
            tile.setTextureIndex(0);
            std::string tileName = dirEntry.path().stem().string();
            std::cout << "Initialised tile: \"" << tileName << "\"" << std::endl;
            tiles.emplace(tileName, tile);
        }
    }
}

bool TileManager::isTile(const std::string& name){
    if (!isInitialised()){
        throw std::runtime_error("No tiles initialised!");
    }
    if (!tiles.contains(name)){
        return false;
    }
    return true;
}

Tile* TileManager::loadTile(const std::string& name){
    if (!isTile(name)){
        throw std::runtime_error("Could not find tile: \"" + name + "\"");
    }
    std::cout << "Loaded tile: \"" << name << "\"" << std::endl;
    return &tiles.at(name);
}

bool TileManager::isInitialised(){
    if (tiles.empty()){
        return false;
    }
    return true;
}
