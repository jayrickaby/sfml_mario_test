//
// Created by jay on 25/01/2026.
//

#include "TileManager.h"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "AnimationManager.h"
#include "GameManager.h"
#include "ManagerUtilities.h"
#include "TextureManager.h"

std::map<std::string, Tile> TileManager::tiles = {};
std::string TileManager::fullPath = "";

void TileManager::initialiseTiles(){
    fullPath = GameManager::getAssetPath() + "models/";

    std::vector files(ManagerUtilities::findFiles(fullPath, {".json"}));

    for (std::string& file : files){
        Tile tile = initialiseTile(file);
        std::string tileName = std::filesystem::path(fullPath + file).stem().string();
        std::cout << "Initialised tile: \"" << tileName << "\"" << std::endl;
        tiles.emplace(tileName, tile);
    }
}

Tile TileManager::initialiseTile(std::string& path){
    Tile tile;
    std::ifstream modelFileData(fullPath + path);
    nlohmann::json modelFile = nlohmann::json::parse(modelFileData);

    if (!modelFile["animation"].empty()){
        tile.setAnimated();
        auto* anims = AnimationManager::loadAnimationFile(modelFile["animation"]);
        tile.addAnimations(anims);
        tile.playAnimation("idleTile");
        tile.setFrameDurationScale(0.15f);
    }
    if (modelFile["textures"].empty()){
        throw std::runtime_error("Model \"" + path + "\" has no textures!");
    }
    for (const auto& texture : modelFile["textures"]){
        std::string textureName = texture.get<std::string>();
        if (TextureManager::isTexture(textureName)){
            tile.addTexture(TextureManager::loadTexture(textureName));
        }
        else{
            throw std::runtime_error("Texture \"" + textureName + "\" not found!");
        }
    }

    tile.setTextureIndex(0);
    return tile;
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
